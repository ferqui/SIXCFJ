import React, { Component } from 'react';
import {
  StyleSheet,
  Text,
  View,
  TouchableOpacity,
  ScrollView } from 'react-native';

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

  render() {
    BluetoothSerial.isConnected()
    .then((res) => this.setState({ connected: res }))
    .catch((err) => Toast.showShortBottom(err.message))
    const { navigate } = this.props.navigation;

    return (
      <View style={styles.container}>
      <Button
        textStyle={{ color: GLOBAL.BLACK }}
        style={styles.buttonRaised}
        title='Arriba'
        onPress={() => this.write('a#')} />
      <View style={{flex: 1, flexDirection: 'row',alignItems: 'center',justifyContent: 'center'}}>
        <Button
          textStyle={{ color: GLOBAL.BLACK }}
          style={styles.buttonRaised}
          title='Izq'
          onPress={() => this.write('i#')} />
        <Button
          textStyle={{ color: GLOBAL.BLACK }}
          style={styles.buttonRaised}
          title='Der'
          onPress={() => this.write('d#')} />
      </View>
      <Button
        textStyle={{ color: GLOBAL.BLACK }}
        style={styles.buttonRaised}
        title='Abajo'
        onPress={() => this.write('b#')} />
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
