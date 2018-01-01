/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 * @flow
 */

import React, { Component } from 'react';
import { StyleSheet, Text, View, Button, ScrollView } from 'react-native';
import { DrawerNavigator, DrawerItems, SafeAreaView  } from 'react-navigation';
import Ionicons from 'react-native-vector-icons/Ionicons';
import ConfigScreen from './ConfigurationScreen';
import MainScreen from './MainScreen';

const HomeScreen = ({ navigation }) => (
  <View style={{ flex: 1, alignItems: 'center', justifyContent: 'center' }}>
    <Text>Home Screen</Text>
    <Button
      onPress={() => navigation.navigate('DrawerToggle')}
      title="Open Drawer"
    />
  </View>
);

const Nav = DrawerNavigator({
  ConfigScreen: {
    screen: ConfigScreen,
    navigationOptions: {
      drawerLabel: 'Configuración',
      drawerIcon: ({ tintColor, focused }) => (
        <Ionicons
          name={focused ? 'ios-home' : 'ios-home-outline'}
          size={20}
          style={{ color: tintColor }}
        />
      ),
    },
  },
  MainScreen: {
    screen: MainScreen,
    navigationOptions: {
      drawerLabel: 'Main Screen',
      drawerIcon: ({ tintColor, focused }) => (
        <Ionicons
          name={focused ? 'ios-person' : 'ios-person-outline'}
          size={20}
          style={{ color: tintColor }}
        />
      ),
    },
  },
},{
  drawerOpenRoute: 'DrawerOpen',
  drawerCloseRoute: 'DrawerClose',
  drawerToggleRoute: 'DrawerToggle',
  initialRouteName: 'ConfigScreen',
  contentOptions: {
    activeBackgroundColor: '#FFFF8D',
    activeTintColor: '#212121',
    inactiveTintColor: '#FFFF8D'
  }
});

export default class App extends Component<{}> {
  render() {
    return <Nav style={styles.container}/>;
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#212121',
  }
});