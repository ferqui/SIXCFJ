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

    // this.timerArriba = null;
    // this.arriba = this.arriba.bind(this);
    // this.stopArriba = this.stopArriba.bind(this);
    //
    // this.timerAbajo = null;
    // this.abajo = this.abajo.bind(this);
    // this.stopAbajo = this.stopAbajo.bind(this);
    //
    // this.timerIzquierda = null;
    // this.izquierda = this.izquierda.bind(this);
    // this.stopIzquierda = this.stopIzquierda.bind(this);
    //
    // this.timerDerecha = null;
    // this.derecha = this.derecha.bind(this);
    // this.stopDerecha = this.stopDerecha.bind(this);
  }
  write (message) {
    if (!this.state.connected) {
      Toast.showShortBottom('You must connect to device first')
    }

    BluetoothSerial.write(message)
    .then((res) => {})
    .catch((err) => Toast.showShortBottom(err.message))
  }

  arriba() {
    this.write('a#');
  }

  abajo() {
    this.write('b#');
  }
  izquierda() {
    this.write('i#');
  }

  derecha() {
    this.write('d#');
  }

  stop() {
    this.write('s#');
  }

  componentDidMount() {
    this._mounted = true;
  }

  componentWillUnmount() {
    this._mounted = false;
  }

  isMounted() {
    return this._mounted
  }

  componentWillMount() {
    BluetoothSerial.isConnected()
    .then((res) => this.setState({ connected: res }))
    .catch((err) => Toast.showShortBottom(err.message))
    BluetoothSerial.on('bluetoothDisabled', () => {if(this.isMounted()) {this.setState({ connected: false })}})
    BluetoothSerial.on('connectionLost', () => {if(this.isMounted()) {this.setState({ connected: false })}})
    BluetoothSerial.on('connectionSuccess', () => {if(this.isMounted()) {this.setState({connected: true})}})
  }

  render() {
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
        {this.state.connected ?
        (
          <View style={{flex: 1, flexDirection: 'column', justifyContent: 'center', alignItems: 'center'}}>
            <TouchableOpacity style={{flex: .5, marginTop: 50}} onPressIn={this.arriba.bind(this)} onPressOut={this.stop.bind(this)}>
              <Ionicons
                name={'md-arrow-dropup-circle'}
                size={100}
                style={{ flex: 1, color: GLOBAL.YELLOW }}
              />
            </TouchableOpacity>
            <View style={{flex: 1, flexDirection: 'row', justifyContent: 'center', alignItems: 'center'}}>
              <TouchableOpacity style={{marginRight: 40}} onPressIn={this.izquierda.bind(this)} onPressOut={this.stop.bind(this)}>
                <Ionicons
                  name={'md-arrow-dropleft-circle'}
                  size={100}
                  style={{ color: GLOBAL.YELLOW }}
                />
              </TouchableOpacity>
              <TouchableOpacity style={{marginLeft: 40}} onPressIn={this.derecha.bind(this)} onPressOut={this.stop.bind(this)}>
                <Ionicons
                  name={'md-arrow-dropright-circle'}
                  size={100}
                  style={{ color: GLOBAL.YELLOW }}
                />
              </TouchableOpacity>
            </View>
            <TouchableOpacity style={{flex: 1}} onPressIn={this.abajo.bind(this)} onPressOut={this.stop.bind(this)}>
              <Ionicons
                name={'md-arrow-dropdown-circle'}
                size={100}
                style={{ color: GLOBAL.YELLOW }}
              />
            </TouchableOpacity>
          </View>
        ) : (
          <Text style={{flex: 1, color: GLOBAL.YELLOW, fontSize: 20}}> Debe conectar un dispositivo </Text>
        )}
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
  button_container: {
    flex: 1,
    justifyContent: 'center',
    flexDirection: 'column'
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
