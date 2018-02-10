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
import GridView from 'react-native-super-grid';

global.Buffer = Buffer
const iconv = require('iconv-lite')

const GLOBAL = require('./Globals');

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
      pos_actual: [4,4],
      connected: false,
      inicio: true,
      bateria: "100"
    }
  }

  componentDidMount() {
    this._mounted = true;
  }

  isMounted() {
    return this._mounted
  }

  handle_disabled() {
    if(this.isMounted()) {this.setState({ connected: false })}
  }

  handle_read(msg) {
    if(this.isMounted()) {
      var data = msg.data.slice(0, -1);
      if (data.startsWith("bt")){
        this.setState({bateria: data.substring(2)})
      }else if (data.startsWith("pos")){
        pos = data.substring(2)
        this.setState({pos_actual: [pos[0]-'0', pos[1]-'0']})
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
      BluetoothSerial.write("s44#")
      .then((res) => {
        Toast.showShortBottom('Successfuly wrote to device')
        this.setState({ connected: true })
      })
      .catch((err) => Toast.showShortBottom(err.message))
      this.setState(previousState => {
        // var p = previousState.paredeh;
        // p[15][20] = previousState.paredeh[15][20] == 1 ? 0 : 1;
        // return { paredeh: p };
        return{inicio: false}
      });
      // Enviar notificación a arduino para empezar (con la posicion actual)
    }else{
      Toast.showShortBottom('You must connect to device first')
    }
  }

  render() {
    const { navigate } = this.props.navigation;
    const Data = [
      { variable: 'Speed: ', value: '-' }, { variable: 'Distance:', value: '-' },
      { variable: 'Battery: ', value: '-' }, { variable: '# of Cells Visited:', value: '-' },
      { variable: 'Time: ', value: '-' }, { variable: 'Time Left: ', value: '-' },
    ];
    var casillas=[];
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
          <View style={{flexDirection: 'row',justifyContent: 'space-between',alignItems: 'center'}}>
            <Text style={{ fontSize: 12, color: GLOBAL.BLACK }}>
              {`Bateria: ${this.state.bateria}%`}
            </Text>
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
          onPress={async() => {this.handle_start.bind(this);}} />
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
