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
  String log = '';

  RawDatagramSocket socket;

  @override
  initState() {
    super.initState();
    RawDatagramSocket.bind(InternetAddress.anyIPv4, 4200).then((_socket) {
      setState(() {
        socket = _socket;
      });
      _socket.listen((e) {
        Datagram dg = _socket.receive();
        setState(() {
          log += dg.toString();
        });
      });
    });
  }

  changeColor(Color color) {
    socket.send(
        [color.red, color.green, color.blue, color.alpha],
        InternetAddress('4.20.4.20'), 4200);
    setState(() => pickerColor = color);
  }

  appendLog(String logLine) {
    setState(() => log += logLine);
  }

  @override
  Widget build(BuildContext context) {
    if (socket == null) {
      return Center(
        child: CircularProgressIndicator(),
      );
    }
    return DefaultTabController(
      length: 2,
      child: Scaffold(
        appBar: AppBar(
            flexibleSpace: SafeArea(
                child: TabBar(tabs: [
          Tab(text: 'picker', icon: Icon(Icons.palette)),
          Tab(text: 'log', icon: Icon(Icons.message)),
        ]))),
        body: TabBarView(physics: NeverScrollableScrollPhysics(), children: [
          ColorPicker(
            pickerColor: pickerColor,
            onColorChanged: changeColor,
            enableLabel: true,
            pickerAreaHeightPercent: 0.8,
          ),
          Text(
            log,
            softWrap: true,
          )
        ]),
      ),
    );
  }
}
