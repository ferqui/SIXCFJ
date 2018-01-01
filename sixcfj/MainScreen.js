import React, { Component } from 'react';
import { View, StyleSheet, TouchableOpacity, Text, Dimensions } from 'react-native';
import Svg,{
    Line,
    Rect,
    Image
} from 'react-native-svg';

import Toast from '@remobile/react-native-toast'
import BluetoothSerial from 'react-native-bluetooth-serial'
import { Buffer } from 'buffer'
global.Buffer = Buffer
const iconv = require('iconv-lite')

const GLOBAL = require('./Globals');

const Button = ({ title, onPress, style, textStyle }) =>
  <TouchableOpacity style={[ styles.button, style ]} onPress={onPress}>
    <Text style={[ styles.buttonText, textStyle ]}>{title.toUpperCase()}</Text>
  </TouchableOpacity>

export default class MainScreen extends Component {
  constructor(props) {
    super(props)

    this.p = new Array(25);
    for (var i = 0; i < 25; i++) {
      this.p[i] = new Array(25);
      for (var j = 0; j < 25; j++) {
        this.p[i][j] = 0;
      }
    }

    this.p[0][1] = 1;
    this.p[1][2] = 1;
    this.p[2][3] = 1;
    this.p[3][4] = 1;
    this.p[5][6] = 1;
    this.p[11][12] = 1;
    this.p[12][13] = 1;
    this.p[15][16] = 1;

    this.p[2][7] = 1;
    this.p[4][9] = 1;
    this.p[5][10] = 1;
    this.p[7][12] = 1;
    this.p[12][17] = 1;
    this.p[10][15] = 1;
    this.p[15][20] = 1;

    this.state = {
      paredeh: this.p,
      pos_actual: [4,4],
      connected: false
    }
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

  handle_button() {
    this.setState(previousState => {
      var p = previousState.paredeh;
      p[15][20] = previousState.paredeh[15][20] == 1 ? 0 : 1;
      return { paredeh: p };
    });
  }

  render() {
    BluetoothSerial.isConnected()
    .then((res) => this.setState({ connected: res }))
    .catch((err) => Toast.showShortBottom(err.message))
    const { navigate } = this.props.navigation;

    var paredes=[];
    var iter=0;
    for (x = 0; x < 5; x += 1) {
      for (y = 0; y < 5; y += 1) {
        if (this.state.paredeh[x+y*5][x+y*5+1] == 1) {
          paredes.push(<Line key={iter} x1={`${(x+1)*20}%`} y1={`${y*20}%`} x2={`${(x+1)*20}%`} y2={`${y*20+20}%`} stroke={GLOBAL.BLACK} strokeWidth="4"/>)
          iter=iter+1;
        }
        if (this.state.paredeh[x+y*5][x+y*5+5] == 1) {
          paredes.push(<Line key={iter} x1={`${x*20}%`} y1={`${(y+1)*20}%`} x2={`${x*20+20}%`} y2={`${(y+1)*20}%`} stroke={GLOBAL.BLACK} strokeWidth="4"/>)
          iter=iter+1;
        }
      }
    }
    return (
      <View style={styles.container}>
        <View style={styles.topBar}>
          <Text style={styles.heading}>Principal</Text>
        </View>
        <Svg style={styles.svg}>
          <Rect
            width="100%"
            height="100%"
            x="0%"
            y="0%"
            fill={GLOBAL.WHITE}
            strokeWidth="0"
          />
          {paredes}
          <Image
              x={`${this.state.pos_actual[0]*20}%`}
              y={`${this.state.pos_actual[1]*20}%`}
              width="20%"
              height="20%"
              preserveAspectRatio="none"
              href={require('./images/gato.jpeg')}
          />
        </Svg>
        <Button
          textStyle={{ color: GLOBAL.BLACK }}
          style={styles.buttonRaised}
          title='Hola'
          onPress={this.handle_button.bind(this)} />
        <Button
          textStyle={{ color: GLOBAL.BLACK }}
          style={styles.buttonRaised}
          title='Adios'
          onPress={() => this.write('Adios#')} />
      </View>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: GLOBAL.BLACK
  },
  svg: {
    flex: 1,
    alignSelf: 'auto'
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
    fontSize: 14
  },
  buttonRaised: {
    backgroundColor: GLOBAL.YELLOW,
    borderRadius: 2,
    elevation: 2
  }
})
