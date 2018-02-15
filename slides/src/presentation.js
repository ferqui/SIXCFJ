// Import React
import React from 'react';

// Import Spectacle Core tags
import {
  Appear,
  BlockQuote,
  Cite,
  Deck,
  Heading,
  ListItem,
  List,
  Quote,
  Slide,
  Image,
  Link,
  Text
} from 'spectacle';

import CodeSlide from 'spectacle-code-slide';

// Import theme
import createTheme from 'spectacle/lib/themes/default';
import stitch1 from './200.gif';
import app from './app.gif';
import algorithm from './algorithm.png';
import caprito from './caprito.png';
import codeino from 'raw-loader!./main.ino';
import logo from './logo.png';

import code from 'raw-loader!../public/assets/code.js';

// Require CSS
require('normalize.css');

const theme = createTheme(
  {
    primary: 'white',
    secondary: '#1F2022',
    tertiary: '#03A9FC',
    quartenary: '#CECECE'
  },
  {
    primary: 'Montserrat',
    secondary: 'Helvetica'
  }
);

export default class Presentation extends React.Component {
  render() {
    return (
      <Deck
        transition={['zoom', 'slide']}
        transitionDuration={500}
        theme={theme}
      >
      <Slide transition={["zoom"]} bgColor="primary">
        <Heading size={2} fit caps lineHeight={1} textColor="secondary">
          Project SIXCFJ
        </Heading>
        <Text margin="10px 0 0" textColor="tertiary" size={5} bold>
          Juan Francisco Cabrera Sánchez
        </Text>
        <Text margin="10px 0 0" textColor="tertiary" size={5} bold>
          Carlos Gallardo Polanco
        </Text>
        <Text margin="10px 0 0" textColor="tertiary" size={5} bold>
          Fernando Manuel Quintana Velázquez
        </Text>
      </Slide>

      <Slide transition={["fade"]} bgColor="tertiary">
      <Heading size={1} fit caps lineHeight={1} textColor="secondary">
        Componentes
      </Heading>
      </Slide>

      <Slide transition={["fade"]} bgColor="primary" textColor="tertiary">
        <Heading size={5} textColor="secondary" caps>Componentes utilizados:</Heading>
        <List>
          <ListItem>3 CNY 70</ListItem>
          <ListItem>1 Ultrasonidos</ListItem>
          <ListItem>2 Sharp</ListItem>
          <ListItem>1 Módulo Bluetooth</ListItem>
          <ListItem>2 Motores DC</ListItem>
          <ListItem>Y pilas, muchas pilas</ListItem>
        </List>
      </Slide>

      <Slide transition={["fade"]} bgColor="tertiary">
      <Heading size={1} fit caps lineHeight={1} textColor="secondary">
        Diseño
      </Heading>
      </Slide>

      <Slide transition={["fade"]} bgColor="primary" textColor="tertiary">
        <Image src={caprito}/>
      </Slide>

      <Slide transition={["fade"]} bgColor="tertiary">
      <Heading size={1} fit caps lineHeight={1} textColor="secondary">
        Algoritmo
      </Heading>
      </Slide>

      <Slide transition={["fade"]} bgColor="primary" textColor="tertiary">
        <Heading size={5} textColor="secondary" caps>Algoritmo:</Heading>
        <List>
          <ListItem> Sistema de prioridades </ListItem>

          <Image src={algorithm}/>
        </List>
      </Slide>

      <Slide transition={["fade"]} bgColor="tertiary">
      <Heading size={1} fit caps lineHeight={1} textColor="secondary">
        Aplicación
      </Heading>
      </Slide>

      <Slide transition={["fade"]} bgColor="primary" textColor="tertiary">
        <Heading size={5} textColor="secondary" caps>Características:</Heading>
        <List fit>
          <ListItem> Permite enlazar directamente el móvil con el Arduino</ListItem>
          <ListItem > Se puede ver el recorrido del robot en el laberinto</ListItem>
          <ListItem > Múltiples parámetros en vista de grid</ListItem>
          <ListItem > A falta de buzzers, buenos son los altavoces del móvil</ListItem>
        </List>
      </Slide>

      <Slide transition={["fade"]} bgColor="primary" textColor="tertiary">
        <Image width="30%" fit src={app} />
      </Slide>

      <Slide transition={["fade"]} bgColor="tertiary">
      <Heading size={1} fit caps lineHeight={1} textColor="secondary">
        Problemas encontrados
      </Heading>
      </Slide>

      <Slide transition={["fade"]} bgColor="primary" textColor="tertiary">
        <Heading size={5} textColor="secondary" caps>Problemas:</Heading>
        <List fit>
          <ListItem> Múltiples problemas a nivel de hardware</ListItem>
          <List>
            <Appear><ListItem> Sensor de Ultrasonidos </ListItem></Appear>
            <Appear><ListItem> Arduino Leonardo </ListItem></Appear>
          </List>
          <Appear><ListItem > En definitiva...</ListItem></Appear>
        </List>
        <Appear><Image fit src={stitch1} /></Appear>
      </Slide>

        <CodeSlide
          bgColor="secondary"
          transition={['fade']}
          lang="cpp"
          code={code}
          ranges={[
            { loc: [0, 7], title: 'Aclaraciones de código' },
            { loc: [5, 11], note: 'Los movimientos se representan por números binarios' },
            { loc: [15, 17], note: 'Se utiliza una pila para almacenar los movimientos' },
            { loc: [71, 84], note: 'Operación AND para tomar la decisión' },
            { loc: [71, 84] },
            { loc: [220, 233], note: 'Se calculan los posibles movimientos' },
          ]}
          showLineNumbers={false}
        />

        <Slide transition={['spin', 'slide']} bgColor="tertiary">
          <Heading size={1} caps fit lineHeight={1.5} textColor="primary">
            ¿Alguna pregunta?
          </Heading>
          <Link href="https://github.com/ferqui/SIXCFJ"><Image width="10%" src={logo}/></Link>
        </Slide>

      </Deck>
    );
  }
}
