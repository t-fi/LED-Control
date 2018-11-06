import 'package:flutter/material.dart';
import 'package:flutter_colorpicker/flutter_colorpicker.dart';

import 'dart:io';

void main() => runApp(RgbApp());

class RgbApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: '',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: MyHomePage(),
    );
  }
}

class MyHomePage extends StatefulWidget {
  MyHomePage({Key key}) : super(key: key);

  @override
  _MyHomePageState createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  Color pickerColor = Color(0xff443a49);

  RawDatagramSocket socket;

  @override
  initState() {
    super.initState();
    RawDatagramSocket.bind(InternetAddress.anyIPv4, 4200).then((_socket) {
      setState(() => socket = _socket);
    });
  }

  changeColor(Color color) {
    var buffer = [color.red, color.green, color.blue, color.alpha];
    socket?.send(buffer, InternetAddress('4.20.4.20'), 4200);
    setState(() => pickerColor = color);
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        body: ColorPicker(
          pickerColor: pickerColor,
          onColorChanged: changeColor,
          enableLabel: true,
          pickerAreaHeightPercent: 1.0,
        ),
    );
  }
}