import React, { Component } from 'react';
import { View, StyleSheet, TouchableOpacity, Text, Dimensions,ScrollView } from 'react-native';
import Svg,{
    Line,
    Rect,
    Image,
    Path
} from 'react-native-svg';
import Ionicons from 'react-native-vector-icons/Ionicons';

import Toast from '@remobile/react-native-toast'
import BluetoothSerial from 'react-native-bluetooth-serial'
import { Buffer } from 'buffer'
import GridView from 'react-native-super-grid'
import Sound from 'react-native-sound'
import {Select, Option} from "react-native-chooser";

global.Buffer = Buffer
const iconv = require('iconv-lite')
const GLOBAL = require('./Globals');

var whoosh;
var whoosh2;
var whoosh3;
var casillas_rec_iter=0;

const Button = ({ title, onPress, style, textStyle }) =>
  <TouchableOpacity style={[ styles.button, style ]} onPress={onPress}>
    <Text style={[ styles.buttonText, textStyle ]}>{title.toUpperCase()}</Text>
  </TouchableOpacity>

class Casilla extends Component {
  constructor(props){
    super(props)
    this.state = {
      color: GLOBAL.WHITE
    }
  }

  render() {
    return(
      <Rect
        width="20%"
        height="20%"
        x={`${this.props.x*20}%`}
        y={`${this.props.y*20}%`}
        fill={this.state.color}
        strokeWidth="0"
        onPressIn={() => {
          this.setState({color: GLOBAL.LIGHT_ORANGE})
        }}
        onPressOut={() => {
            this.setState({color: GLOBAL.WHITE})
            this.props.press(this.props.x,this.props.y);
          }
        }
      />
    );
  }
}

class CasillaRec extends Component {
  constructor(props){
    super(props)
    this.state = {
      color: GLOBAL.ORANGE
    }
  }

  render() {
    return(
      <Rect
        width="20%"
        height="20%"
        x={`${this.props.x*20}%`}
        y={`${this.props.y*20}%`}
        fill={this.state.color}
        strokeWidth="0"
      />
    );
  }
}

export default class MainScreen extends Component {
  constructor(props) {
    super(props)

    this.p = new Array(25);
    for (var i = 0; i < 25; i++) {
      this.p[i] = new Array(25);
      for (var j = 0; j < 25; j++) {
        this.p[i][j] = 1;
      }
    }

    this.state = {
      paredeh: this.p,
      dir: 0,
      pos_actual: [4,4],
      connected: false,
      inicio: true,
      bateria: "-",
      distancia: 0,
      velocidad: 0,
      ncasillas: 1,
      tiempo: 0,
      tiempo_rest: 300,
      musicIni: "",
      musicIniName: "Start song",
      musicFin: "",
      musicFinName: "End Song",
      intervalId: null,
      contador: 0,
      casillas_recorridas: [],

    }
  }

  componentDidMount() {
    this._mounted = true;
    var intervalId = setInterval(this.ticktock.bind(this),1000);
    this.setState({intervalId:intervalId});
  }

  isMounted() {
    return this._mounted
  }

  handle_disabled() {
    if(this.isMounted()) {this.setState({ connected: false })}
  }

  ticktock(){
    if(!this.state.inicio){
      this.setState({ tiempo: this.state.tiempo+1, tiempo_rest: this.state.tiempo_rest-1 })
    }
  }

  handle_read(msg) {
    if(this.isMounted()) {
      var data = msg.data.slice(0, -1);
      var pos = this.state.pos_actual;
      var dir = this.state.dir;

      if (data.startsWith("music1")){
        // Enable playback in silence mode
        Sound.setCategory('Playback',true);
         whoosh = new Sound(this.state.musicIni, Sound.MAIN_BUNDLE, (error) => {
          if (error) {
            Toast.showShortBottom('failed to load the sound');
          }
          else{
            whoosh.play();
          }
        });

      }
      else if(data.startsWith("congrats")){
        this.setState({inicio: false});
        whoosh.pause();
        // Enable playback in silence mode
        Sound.setCategory('Playback',true);
        whoosh2 = new Sound('jingle.mp3', Sound.MAIN_BUNDLE, (error) => {
         if (error) {
           Toast.showShortBottom('failed to load the sound');
         }
         else{
           whoosh2.play();
         }
       });
     }else if(data.startsWith("music2")){
       // Enable playback in silence mode
       whoosh2.stop();
       Sound.setCategory('Playback',true);
        whoosh3 = new Sound(this.state.musicFin, Sound.MAIN_BUNDLE, (error) => {
         if (error) {
           Toast.showShortBottom('failed to load the sound');
         }
         else{
           whoosh3.play();
         }
       });
     }
      else if (data.startsWith("bt")){
        this.setState({bateria: data.substring(2)})
      }
      if(data.startsWith("fin")){
        whoosh2 = new Sound('jingle.mp3', Sound.MAIN_BUNDLE, (error) => {
         if (error) {
           Toast.showShortBottom('failed to load the sound');
         }
         else{
           whoosh2.play();
         }
       });
      }
      if(data.startsWith("end")){
        whoosh.release();whoosh2.release();whoosh3.release();
      }
      if (data.startsWith("dist")){
        this.setState(
          {distancia: this.state.distancia + Math.abs(data.substring(4)),
           contador: this.state.contador + 1,
           velocidad: (this.state.distancia/(this.state.tiempo))})
      }

      if(data.startsWith("casilla")){
        this.setState({ncasillas: this.state.ncasillas + 1})
      }
      // Move RIGHT
      if(data.startsWith("0")){
        if(dir==0){ // Looking FORWARD
          pos[0] += 1;
          dir = 3;
        }else if(dir==1){ // Looking BACKWARD
          pos[0] -= 1;
          dir = 2;
        }else if(dir==2){ // Looking LEFT
          pos[1] -= 1;
          dir = 0;
        }else if(dir==3){ // Looking RIGHT
          pos[1] += 1;
          dir = 1;
        }
        var casillas_rec = this.state.casillas_recorridas;
        casillas_rec.push(<CasillaRec key={casillas_rec_iter} x={pos[0]} y={pos[1]} />)
        casillas_rec_iter=casillas_rec_iter+1;
        this.setState({pos_actual: [pos[0], pos[1]], casillas_recorridas: casillas_rec})
        this.setState({dir: dir})
      }
      // Move FORWARD
      if(data.startsWith("1")){
        if(dir==0){
          pos[1] -= 1;
        }else if(dir==1){
          pos[1] += 1;
        }else if(dir==2){
          pos[0] -= 1;
        }else if(dir==3){
          pos[0] += 1;
        }
        var casillas_rec = this.state.casillas_recorridas;
        casillas_rec.push(<CasillaRec key={casillas_rec_iter} x={pos[0]} y={pos[1]} />)
        casillas_rec_iter=casillas_rec_iter+1;
        this.setState({pos_actual: [pos[0], pos[1]], casillas_recorridas: casillas_rec})
      }
      // Move LEFT
      if(data[0]=="2"){
        if(dir==0){
          pos[0] -= 1;
          dir = 2;
        }else if(dir==1){
          pos[0] += 1;
          dir = 3;
        }else if(dir==2){
          pos[1] += 1;
          dir = 1;
        }else if(dir==3){
          pos[1] -= 1;
          dir = 0;
        }
        var casillas_rec = this.state.casillas_recorridas;
        casillas_rec.push(<CasillaRec key={casillas_rec_iter} x={pos[0]} y={pos[1]} />)
        casillas_rec_iter=casillas_rec_iter+1;
        this.setState({pos_actual: [pos[0], pos[1]], casillas_recorridas: casillas_rec})
        this.setState({dir: dir})
      }
      // Move BACKWARD
      if(data[0]=="3"){
        if(dir==0){
          pos[1] += 1;
        }else if(dir==1){
          pos[1] -= 1;
        }else if(dir==2){
          pos[0] += 1;
        }else if(dir==3){
          pos[0] -= 1;
        }
        var casillas_rec = this.state.casillas_recorridas;
        casillas_rec.pop();
        casillas_rec_iter=casillas_rec_iter+1;
        this.setState({pos_actual: [pos[0], pos[1]], casillas_recorridas: casillas_rec})
      }
      // Move LEFT_BACKWARD
      if(data[0]=="4"){
        if(dir==0){
          pos[1] += 1;
          dir = 3
        }else if(dir==1){
          pos[1] -= 1;
          dir = 2
        }else if(dir==2){
          pos[0] += 1;
          dir = 0;
        }else if(dir==3){
          pos[0] -= 1;
          dir = 1;
        }
        var casillas_rec = this.state.casillas_recorridas;
        casillas_rec.pop();
        casillas_rec_iter=casillas_rec_iter+1;
        this.setState({pos_actual: [pos[0], pos[1]], casillas_recorridas: casillas_rec})
        this.setState({dir: dir})
      }
      // Move RIGHT_BACKWARD
      if(data[0]=="5"){
        if(dir==0){
          pos[1] += 1;
          dir = 2
        }else if(dir==1){
          pos[1] -= 1;
          dir = 3
        }else if(dir==2){
          pos[0] += 1;
          dir = 1;
        }else if(dir==3){
          pos[0] -= 1;
          dir = 0;
        }
        var casillas_rec = this.state.casillas_recorridas;
        casillas_rec.pop();
        casillas_rec_iter=casillas_rec_iter+1;
        this.setState({pos_actual: [pos[0], pos[1]], casillas_recorridas: casillas_rec})
        this.setState({dir: dir})
      }
    }

  }

  componentWillMount() {
    BluetoothSerial.withDelimiter('#')
    BluetoothSerial.isConnected()
    .then((res) => this.setState({ connected: res }))
    .catch((err) => Toast.showShortBottom(err.message))
    BluetoothSerial.on('connectionLost', this.handle_disabled.bind(this))
    BluetoothSerial.on('connectionSuccess', this.handle_disabled.bind(this))
    BluetoothSerial.on("read", this.handle_read.bind(this))
  }

  componentWillUnmount(){
    this._mounted = false;
    BluetoothSerial.removeListener('connectionLost', this.handle_disabled.bind(this))
    BluetoothSerial.removeListener('connectionSuccess', this.handle_disabled.bind(this))
    BluetoothSerial.removeListener('read', this.handle_read.bind(this))
    clearInterval(this.state.intervalId);
  }

  write (message) {
    if (!this.state.connected) {
      Toast.showShortBottom('You must connect to device first')
    }

    BluetoothSerial.write(message)
    .then((res) => {
      Toast.showShortBottom('Successfuly wrote to device')
      this.setState({ connected: true })
    })
    .catch((err) => Toast.showShortBottom(err.message))
  }

  handle_start() {
    if (this.state.connected){
      BluetoothSerial.write("start#")
      .then((res) => {
        Toast.showShortBottom('Successfuly wrote to device')
        this.setState({ connected: true })

      })
      .catch((err) => Toast.showShortBottom(err.message))
      this.setState(previousState => {
        var casillas_rec = previousState.casillas_recorridas;
        casillas_rec.push(<CasillaRec key={casillas_rec_iter} x={previousState.pos_actual[0]} y={previousState.pos_actual[1]} />)
        casillas_rec_iter=casillas_rec_iter+1;
        return{inicio: false, casillas_recorridas: casillas_rec}
      });
      // Enviar notificación a arduino para empezar (con la posicion actual)
    }else{
      Toast.showShortBottom('You must connect to device first')
    }
  }

  onSelectIni(value, label) {
    this.setState({musicIni : value, musicIniName: label});
  }

  onSelectFin(value, label) {
    this.setState({musicFin : value, musicFinName: label});
  }

  render() {
    const { navigate } = this.props.navigation;
    const Data = [
      { variable: 'Speed: ', value: this.state.velocidad.toFixed(2) + " cm/s"}, { variable: 'Distance:', value: this.state.distancia + " cm" },
      { variable: 'Battery: ', value: this.state.bateria+ " V" }, { variable: '# of Cells Visited:', value: this.state.ncasillas },
      { variable: 'Time: ', value: this.state.tiempo + " s" }, { variable: 'Time Left: ', value: this.state.tiempo_rest+ " s" },
    ];
    var casillas=[];
    var casillas_rec=[];
    var paredes=[];
    var iter=0;
    for (xx = 0; xx < 5; xx += 1) {
      for (yy = 0; yy < 5; yy += 1) {
        if (this.state.paredeh[xx+yy*5][xx+yy*5+1] == 1) {
          paredes.push(<Line key={iter} x1={`${(xx+1)*20}%`} y1={`${yy*20}%`} x2={`${(xx+1)*20}%`} y2={`${yy*20+20}%`} stroke={GLOBAL.BLACK} strokeWidth="4"/>)
          iter=iter+1;
        }
        if (this.state.paredeh[xx+yy*5][xx+yy*5+5] == 1) {
          paredes.push(<Line key={iter} x1={`${xx*20}%`} y1={`${(yy+1)*20}%`} x2={`${xx*20+20}%`} y2={`${(yy+1)*20}%`} stroke={GLOBAL.BLACK} strokeWidth="4"/>)
          iter=iter+1;
        }

        if (this.state.inicio){
          casillas.push(<Casilla key={iter} x={xx} y={yy} press={(data1,data2) => this.setState({pos_actual: [data1,data2]})} />)
        }
      }
    }

    for (let element of this.state.casillas_recorridas) {
      casillas_rec.push(element);
    }

    return (
      <View style={styles.container}>
        <View style={styles.topBar}>
          <View style={{flex: 1, flexDirection: 'row', alignItems: 'flex-start'}}>
            <TouchableOpacity style={{paddingHorizontal: 16}} onPress={() => {this.props.navigation.navigate('DrawerToggle');}}>
              <Ionicons
                name={'md-menu'}
                size={25}
                style={{ color: GLOBAL.BLACK }}
              />
            </TouchableOpacity>
            <Text style={styles.heading}>Principal</Text>
          </View>
        </View>
        <ScrollView>
        <View style={styles.svgContainer}>
          <Svg style={styles.svg}>
            <Rect
              width="100%"
              height="100%"
              x="0%"
              y="0%"
              fill={GLOBAL.WHITE}
              strokeWidth="0"
            />
            {casillas}
            {casillas_rec}
            <Line key={iter} x1="20%" y1="0%" x2="20%" y2="100%" stroke={GLOBAL.GRAY} strokeWidth="2"/>
            <Line key={iter} x1="40%" y1="0%" x2="40%" y2="100%" stroke={GLOBAL.GRAY} strokeWidth="2"/>
            <Line key={iter} x1="60%" y1="0%" x2="60%" y2="100%" stroke={GLOBAL.GRAY} strokeWidth="2"/>
            <Line key={iter} x1="80%" y1="0%" x2="80%" y2="100%" stroke={GLOBAL.GRAY} strokeWidth="2"/>

            <Line key={iter} x1="0%" y1="20%" x2="100%" y2="20%" stroke={GLOBAL.GRAY} strokeWidth="2"/>
            <Line key={iter} x1="0%" y1="40%" x2="100%" y2="40%" stroke={GLOBAL.GRAY} strokeWidth="2"/>
            <Line key={iter} x1="0%" y1="60%" x2="100%" y2="60%" stroke={GLOBAL.GRAY} strokeWidth="2"/>
            <Line key={iter} x1="0%" y1="80%" x2="100%" y2="80%" stroke={GLOBAL.GRAY} strokeWidth="2"/>
            {paredes}
            <Image
                x={`${this.state.pos_actual[0]*20}%`}
                y={`${this.state.pos_actual[1]*20}%`}
                width="20%"
                height="20%"
                preserveAspectRatio="none"
                href={{uri: 'asset:/coche.png'}}
            />
          </Svg>
        </View>
        <View style={{flex: 1, flexDirection: 'row', alignItems: 'flex-start'}}>
        <Select
            onSelect = {this.onSelectIni.bind(this)}
            defaultText  = {this.state.musicIniName}
            style = {{}}
            textStyle = {{color: "white"}}
            backdropStyle  = {{backgroundColor : "#d3d5d6"}}
            optionListStyle = {{backgroundColor : "#F5FCFF"}}
          >
          <Option value = "music1.mp3">Never Gonna Give You Up</Option>
          <Option value = "music2.mp3">Piña Colada</Option>
          <Option value = "music3.mp3">September</Option>
          <Option value = "music4.mp3">Superstition</Option>

        </Select>
        <Select
            onSelect = {this.onSelectFin.bind(this)}
            defaultText  = {this.state.musicFinName}
            style = {{}}
            textStyle = {{color: "white"}}
            backdropStyle  = {{backgroundColor : "#d3d5d6"}}
            optionListStyle = {{backgroundColor : "#F5FCFF"}}
          >
          <Option value = "music1.mp3">Never Gonna Give You Up</Option>
          <Option value = "music2.mp3">Piña Colada</Option>
          <Option value = "music3.mp3">September</Option>
          <Option value = "music4.mp3">Superstition</Option>

        </Select>
      </View>

        <View style={styles.svgContainer}>
        <GridView
          itemDimension={100}
          items={Data}
          style={styles.gridView}
          renderItem={item => (
            <View style={[styles.itemContainer, { backgroundColor:  '#FFFF8D' }]}>
              <Text style={styles.itemType}>{item.variable}</Text>
              <Text> </Text>
              <Text style={styles.itemValue}>{item.value}</Text>
            </View>
          )}
        />
        </View>
        </ScrollView>
        <Button
          textStyle={{ color: GLOBAL.BLACK }}
          style={styles.buttonRaised}
          title='START'
          onPress={this.handle_start.bind(this)} />
      </View>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: GLOBAL.BLACK
  },
  svgContainer: {
    flex: 1,
    flexDirection: 'row',
  },
  svg: {
    flex: 1,
    flexDirection: 'row',
    alignSelf: 'stretch',
    aspectRatio: 1
  },
  topBar: {
    height: 56,
    paddingHorizontal: 16,
    flexDirection: 'row',
    justifyContent: 'space-between',
    alignItems: 'center' ,
    elevation: 6,
    backgroundColor: GLOBAL.YELLOW
  },
  heading: {
    fontWeight: 'bold',
    fontSize: 16,
    alignSelf: 'center',
    color: GLOBAL.BLACK
  },
  button: {
    height: 36,
    margin: 5,
    paddingHorizontal: 16,
    alignItems: 'center',
    justifyContent: 'center'
  },
  buttonText: {
    color: GLOBAL.YELLOW,
    fontWeight: 'bold',
    fontSize: 24
  },
  buttonRaised: {
    backgroundColor: GLOBAL.YELLOW,
    borderRadius: 2,
    elevation: 2
  },
  gridView: {
    paddingTop: 20,
    flex: 1,
  },
  itemContainer: {
    justifyContent: 'flex-start',
    borderRadius: 5,
    padding: 10,
    height: 150,
  },
  itemType: {
    fontSize: 16,
    color: '#000',
    fontWeight: '600',
    textAlign: 'center',
  },
  itemValue: {
    fontWeight: '600',
    fontSize: 20,
    color: '#000',
    textAlign: 'center',
  },
})
