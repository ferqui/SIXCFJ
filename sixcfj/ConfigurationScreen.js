import React, { Component } from 'react';
import {
  Platform,
  ScrollView,
  StyleSheet,
  Switch,
  Text,
  TouchableOpacity,
  TouchableHighlight,
  View,
  Modal,
  ActivityIndicator
} from 'react-native';

const GLOBAL = require('./Globals');

import Toast from '@remobile/react-native-toast'
import BluetoothSerial from 'react-native-bluetooth-serial'
import { Buffer } from 'buffer'
global.Buffer = Buffer
const iconv = require('iconv-lite')
import Ionicons from 'react-native-vector-icons/Ionicons';

const Button = ({ title, onPress, style, textStyle }) =>
  <TouchableOpacity style={[ styles.button, style ]} onPress={onPress}>
    <Text style={[ styles.buttonText, textStyle ]}>{title.toUpperCase()}</Text>
  </TouchableOpacity>

const DeviceList = ({ devices, connectedId, showConnectedIcon, onDevicePress }) =>
  <ScrollView style={styles.container}>
    <View style={styles.listContainer}>
      {devices.map((device, i) => {
        return (
          <TouchableHighlight
            underlayColor={GLOBAL.LIGHT_ORANGE}
            key={`${device.id}_${i}`}
            style={styles.listItem} onPress={() => onDevicePress(device)}>
            <View style={{ flexDirection: 'row' }}>
              <View style={{ justifyContent: 'space-between', flexDirection: 'row', alignItems: 'center' }}>
                <Text style={{ fontWeight: 'bold', color: GLOBAL.WHITE }}>{device.name}</Text>
                <Text style={{ color: GLOBAL.WHITE }}>{`<${device.id}>`}</Text>
              </View>
            </View>
          </TouchableHighlight>
        )
      })}
    </View>
  </ScrollView>

export default class ConfigScreen extends Component {
  constructor (props) {
    super(props)
    this.state = {
      isEnabled: false,
      discovering: false,
      devices: [],
      unpairedDevices: [],
      connected: false,
      section: 0,
      data: "Hola",
      timeTick: 0
    }
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

  componentWillMount () {
    Promise.all([
      BluetoothSerial.isEnabled(),
      BluetoothSerial.list()
    ])
    .then((values) => {
      const [ isEnabled, devices ] = values
      this.setState({ isEnabled, devices })
    })

    BluetoothSerial.withDelimiter('#')
    BluetoothSerial.on('bluetoothEnabled', () => {
      if(this.isMounted()) {
        Toast.showShortBottom('Bluetooth enabled')
        Promise.all([
          BluetoothSerial.isEnabled(),
          BluetoothSerial.list()
        ])
        .then((values) => {
          const [ isEnabled, devices ] = values
          this.setState({ isEnabled, devices })
        })
      }
    })
    BluetoothSerial.on('bluetoothDisabled', () => Toast.showShortBottom('Bluetooth disabled'))
    BluetoothSerial.on('error', (err) => console.log(`Error: ${err.message}`))
    BluetoothSerial.on('connectionLost', () => {
      if(this.isMounted()) {
        if (this.state.device) {
          Toast.showShortBottom(`Connection to device ${this.state.device.name} has been lost`);
        }
        this.setState({ connected: false });
        this.interval && clearInterval(this.interval);
        this.setState({ connectedId: ""});
      }
    })
  }

  /**
   * [android]
   * request enable of bluetooth from user
   */
  requestEnable () {
    BluetoothSerial.requestEnable()
    .then((res) => this.setState({ isEnabled: true }))
    .catch((err) => Toast.showShortBottom(err.message))
  }

  /**
   * [android]
   * enable bluetooth on device
   */
  enable () {
    BluetoothSerial.enable()
    .then((res) => this.setState({ isEnabled: true }))
    .catch((err) => Toast.showShortBottom(err.message))
  }

  /**
   * [android]
   * disable bluetooth on device
   */
  disable () {
    BluetoothSerial.disable()
    .then((res) => this.setState({ isEnabled: false }))
    .catch((err) => Toast.showShortBottom(err.message))
  }

  /**
   * [android]
   * toggle bluetooth
   */
  toggleBluetooth (value) {
    if (value === true) {
      this.enable()
    } else {
      this.disable()
    }
  }

  /**
   * [android]
   * Discover unpaired devices, works only in android
   */
  discoverUnpaired () {
    if (this.state.discovering) {
      return false
    } else {
      this.setState({ discovering: true })
      BluetoothSerial.discoverUnpairedDevices()
      .then((unpairedDevices) => {
        this.setState({ unpairedDevices, discovering: false })
      })
      .catch((err) => Toast.showShortBottom(err.message))
    }
  }

  /**
   * [android]
   * Discover unpaired devices, works only in android
   */
  cancelDiscovery () {
    if (this.state.discovering) {
      BluetoothSerial.cancelDiscovery()
      .then(() => {
        this.setState({ discovering: false })
      })
      .catch((err) => Toast.showShortBottom(err.message))
    }
  }

  /**
   * [android]
   * Pair device
   */
  pairDevice (device) {
    BluetoothSerial.pairDevice(device.id)
    .then((paired) => {
      if (paired) {
        Toast.showShortBottom(`Device ${device.name} paired successfully`)
        const devices = this.state.devices
        devices.push(device)
        this.setState({ devices, unpairedDevices: this.state.unpairedDevices.filter((d) => d.id !== device.id) })
      } else {
        Toast.showShortBottom(`Device ${device.name} pairing failed`)
      }
    })
    .catch((err) => Toast.showShortBottom(err.message))
  }

  /**
   * Connect to bluetooth device by id
   * @param  {Object} device
   */
  connect (device) {
    this.setState({ connecting: true })
    BluetoothSerial.connect(device.id)
    .then((res) => {
      Toast.showShortBottom(`Connected to device ${device.name}`)
      this.setState({ device, connected: true, connecting: false })
      this.props.navigation.navigate('Game');
    })
    .catch((err) => Toast.showShortBottom(err.message))
  }

  /**
   * Disconnect from bluetooth device
   */
  disconnect () {
    BluetoothSerial.disconnect()
    .then(() => {this.setState({ connected: false });this.interval && clearInterval(this.interval);})
    .catch((err) => Toast.showShortBottom(err.message))
  }

  /**
   * Toggle connection when we have active device
   * @param  {Boolean} value
   */
  toggleConnect (value) {
    if (value === true && this.state.device) {
      this.connect(this.state.device)
    } else {
      this.disconnect()
    }
  }

  /**
   * Write message to device
   * @param  {String} message
   */
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

  onDevicePress (device) {
    if (this.state.section === 0) {
      this.connect(device)
    } else {
      this.pairDevice(device)
    }
  }

  writePackets (message, packetSize = 64) {
    const toWrite = iconv.encode(message, 'cp852')
    const writePromises = []
    const packetCount = Math.ceil(toWrite.length / packetSize)

    for (var i = 0; i < packetCount; i++) {
      const packet = new Buffer(packetSize)
      packet.fill(' ')
      toWrite.copy(packet, 0, i * packetSize, (i + 1) * packetSize)
      writePromises.push(BluetoothSerial.write(packet))
    }

    Promise.all(writePromises)
    .then((result) => {
    })
  }

  render () {
    const activeTabStyle = { borderBottomWidth: 6, borderColor: GLOBAL.ORANGE }
    return (
      <View style={{ flex: 1, backgroundColor: GLOBAL.BLACK }}>
        <View style={styles.topBar}>
          <View style={{flex: 1, flexDirection: 'row', alignItems: 'flex-start'}}>
            <TouchableOpacity style={{paddingHorizontal: 16}} onPress={() => {this.props.navigation.navigate('DrawerToggle');}}>
              <Ionicons
                name={'md-menu'}
                size={25}
                style={{ color: GLOBAL.BLACK }}
              />
            </TouchableOpacity>
            <Text style={styles.heading}>Configuración</Text>
          </View>
          {Platform.OS === 'android'
          ? (
            <View style={styles.enableInfoWrapper}>
              <Text style={{ fontSize: 12, color: GLOBAL.BLACK }}>
                {this.state.isEnabled ? 'disable' : 'enable'}
              </Text>
              <Switch
                thumbTintColor={GLOBAL.ORANGE}
                onTintColor={GLOBAL.LIGHT_ORANGE}
                onValueChange={this.toggleBluetooth.bind(this)}
                value={this.state.isEnabled} />
            </View>
          ) : null}
        </View>

        {Platform.OS === 'android'
        ? (
          <View style={[styles.topBar, { justifyContent: 'center', paddingHorizontal: 0 }]}>
            <TouchableOpacity style={[styles.tab, this.state.section === 0 && activeTabStyle]} onPress={() => this.setState({ section: 0 })}>
              <Text style={{ fontSize: 14, color: GLOBAL.BLACK }}>PAIRED DEVICES</Text>
            </TouchableOpacity>
            <TouchableOpacity style={[styles.tab, this.state.section === 1 && activeTabStyle]} onPress={() => this.setState({ section: 1 })}>
              <Text style={{ fontSize: 14, color: GLOBAL.BLACK }}>UNPAIRED DEVICES</Text>
            </TouchableOpacity>
          </View>
        ) : null}
        {this.state.discovering && this.state.section === 1
        ? (
          <View style={{ flex: 1, alignItems: 'center', justifyContent: 'center' }}>
            <ActivityIndicator
              style={{ marginBottom: 15 }}
              size={60} />
            <Button
              textStyle={{ color: GLOBAL.BLACK }}
              style={styles.buttonRaised}
              title='Cancel Discovery'
              onPress={() => this.cancelDiscovery()} />
          </View>
        ) : (
          <DeviceList
            showConnectedIcon={this.state.section === 0}
            connectedId={this.state.device && this.state.device.id}
            devices={this.state.section === 0 ? this.state.devices : this.state.unpairedDevices}
            onDevicePress={(device) => this.onDevicePress(device)} />
        )}


        <View style={{ alignSelf: 'flex-end', height: 52 }}>
          <ScrollView
            horizontal
            contentContainerStyle={styles.fixedFooter}>
            {Platform.OS === 'android' && this.state.section === 1
            ? (
              <Button
                title={this.state.discovering ? '... Discovering' : 'Discover devices'}
                onPress={this.discoverUnpaired.bind(this)} />
            ) : null}
            {Platform.OS === 'android' && !this.state.isEnabled
            ? (
              <Button
                title='Request enable'
                onPress={() => this.requestEnable()} />
            ) : null}
          </ScrollView>
        </View>
      </View>
    )
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 0.9,
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
  enableInfoWrapper: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    alignItems: 'center'
  },
  tab: {
    alignItems: 'center',
    flex: 0.5,
    height: 56,
    justifyContent: 'center',
    borderBottomWidth: 6,
    borderColor: 'transparent'
  },
  connectionInfoWrapper: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    alignItems: 'center',
    paddingHorizontal: 25
  },
  connectionInfo: {
    fontWeight: 'bold',
    alignSelf: 'center',
    fontSize: 18,
    marginVertical: 10,
    color: '#238923'
  },
  listContainer: {
    borderColor: GLOBAL.LIGHT_ORANGE,
    borderTopWidth: 0.5
  },
  listItem: {
    flex: 1,
    height: 48,
    paddingHorizontal: 16,
    borderColor: GLOBAL.LIGHT_ORANGE,
    borderBottomWidth: 0.5,
    justifyContent: 'center'
  },
  fixedFooter: {
    flexDirection: 'row',
    justifyContent: 'center',
    alignItems: 'center',
    borderTopWidth: 1,
    borderTopColor: '#ddd'
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
