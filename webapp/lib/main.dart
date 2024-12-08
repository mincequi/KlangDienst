import 'package:KlangDienst/services/eq_service.dart';
import 'package:KlangDienst/services/filter_service.dart';
import 'package:KlangDienst/services/websocket_service.dart';
import 'package:flutter/material.dart';
import 'package:get/get.dart';

import 'views/home_page.dart';

void main() {
  Get.put(FilterService());
  Get.put(EqService());
  Get.put(WebsocketService());
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return GetMaterialApp(
      title: 'KlangDienst',
      debugShowCheckedModeBanner: false,
      theme: ThemeData(
        colorScheme: _customColorScheme(),
        useMaterial3: true,
        /*dropdownMenuTheme: const DropdownMenuThemeData(
          textStyle: TextStyle(color: Colors.amber, fontSize: 12),
        ),*/
      ),
      darkTheme: ThemeData(
        colorScheme: _customColorScheme(),
        useMaterial3: true, // For Material 3 design
        iconButtonTheme: IconButtonThemeData(
          style: ButtonStyle(
            shape: WidgetStatePropertyAll(
              RoundedRectangleBorder(
                borderRadius: BorderRadius.circular(8), // Custom corner radius
              ),
            ),
          ),
        ),
        dropdownMenuTheme: const DropdownMenuThemeData(
          //textStyle: TextStyle(color: Colors.amber, fontSize: 12),
          inputDecorationTheme: InputDecorationTheme(
            contentPadding: EdgeInsets.symmetric(horizontal: 16),
            //filled: true,
            //isDense: true,
            border: InputBorder.none,
            /*border: OutlineInputBorder(
              borderRadius: BorderRadius.all(Radius.circular(8)),
            ),*/
          ),
        ),
      ),
      themeMode: ThemeMode.dark,
      home: HomePage(title: 'KlangDienst'),
      //home: HomePageTest(),
    );
  }

  ColorScheme _customColorScheme() {
    // Generate a color scheme using separate seed colors
    final primaryScheme = ColorScheme.fromSeed(
        seedColor: Colors.red, brightness: Brightness.dark);
    final secondaryScheme = ColorScheme.fromSeed(
        seedColor: Colors.cyan, brightness: Brightness.dark);

    // Merge primary and secondary colors into one color scheme
    return primaryScheme; /*.copyWith(
        secondary: secondaryScheme.primary,
        secondaryContainer: secondaryScheme.primaryContainer);*/
  }
}
