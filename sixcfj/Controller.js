import React, { Component } from 'react';
import {
  StyleSheet,
  Text,
  View,
  TouchableOpacity,
  ScrollView } from 'react-native';
import Ionicons from 'react-native-vector-icons/Ionicons';

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

export default class Controller extends Component {
  constructor(props) {
    super(props)
    this.state = {
      connected: false,
      width: 0,
      height: 0
    }

    this.timerArriba = null;
    this.arriba = this.arriba.bind(this);
    this.stopArriba = this.stopArriba.bind(this);

    this.timerAbajo = null;
    this.abajo = this.abajo.bind(this);
    this.stopAbajo = this.stopAbajo.bind(this);

    this.timerIzquierda = null;
    this.izquierda = this.izquierda.bind(this);
    this.stopIzquierda = this.stopIzquierda.bind(this);

    this.timerDerecha = null;
    this.derecha = this.derecha.bind(this);
    this.stopDerecha = this.stopDerecha.bind(this);
  }
  write (message) {
    if (!this.state.connected) {
      Toast.showShortBottom('You must connect to device first')
    }

    BluetoothSerial.write(message)
    .then((res) => {
      this.setState({ connected: true })
    })
    .catch((err) => Toast.showShortBottom(err.message))
  }

  arriba() {
    this.write('a#');
    //this.timerArriba = setTimeout(this.arriba, 200);
  }

  stopArriba() {
    //clearTimeout(this.timerArriba);
    this.write('s#');
  }

  abajo() {
    this.write('b#');
    //this.timerAbajo = setTimeout(this.abajo, 200);
  }

  stopAbajo() {
    //clearTimeout(this.timerAbajo);
    this.write('s#');
  }

  izquierda() {
    this.write('i#');
    //this.timerIzquierda = setTimeout(this.izquierda, 200);
  }

  stopIzquierda() {
    //clearTimeout(this.timerIzquierda);
    this.write('s#');
  }

  derecha() {
    this.write('d#');
    //this.timerDerecha = setTimeout(this.derecha, 200);
  }

  stopDerecha() {
    this.write('s#');
    //clearTimeout(this.timerDerecha);
  }

  render() {
    BluetoothSerial.isConnected()
    .then((res) => this.setState({ connected: res }))
    .catch((err) => Toast.showShortBottom(err.message))
    const { navigate } = this.props.navigation;

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
        <View style={{flex: 1, flexDirection: 'column',alignItems: 'center',justifyContent: 'center'}}>
          <TouchableOpacity onPressIn={this.arriba} onPressOut={this.stopArriba}>
            <Text style={{color: GLOBAL.YELLOW, fontWeight: 'bold', fontSize: 14}}>Arriba</Text>
          </TouchableOpacity>
          <View style={{flex: 1, flexDirection: 'row',alignItems: 'center',justifyContent: 'center'}}>
            <TouchableOpacity onPressIn={this.izquierda} onPressOut={this.stopIzquierda}>
              <Text style={{color: GLOBAL.YELLOW, fontWeight: 'bold', fontSize: 14}}>Izquierda</Text>
            </TouchableOpacity>
            <TouchableOpacity onPressIn={this.derecha} onPressOut={this.stopDerecha}>
              <Text style={{color: GLOBAL.YELLOW, fontWeight: 'bold', fontSize: 14}}>Derecha</Text>
            </TouchableOpacity>
          </View>
          <TouchableOpacity onPressIn={this.abajo} onPressOut={this.stopAbajo}>
            <Text style={{color: GLOBAL.YELLOW, fontWeight: 'bold', fontSize: 14}}>Abajo</Text>
          </TouchableOpacity>
        </View>
      </View>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: GLOBAL.BLACK
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
