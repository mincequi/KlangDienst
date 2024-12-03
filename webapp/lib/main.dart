import 'package:KlangDienst/home_test.dart';
import 'package:KlangDienst/views/eq_view.dart';
import 'package:flutter/material.dart';
import 'package:get/get.dart';

import 'controllers/eq_controller.dart';
import 'views/filter_view.dart';
import 'views/filter_screen.dart';
import 'views/home_page.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    final EqController controller = Get.put(EqController());

    return GetMaterialApp(
      title: 'KlangDienst',
      debugShowCheckedModeBanner: false,
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.deepPurple),
        useMaterial3: true,
        dropdownMenuTheme: const DropdownMenuThemeData(
          textStyle: TextStyle(color: Colors.amber, fontSize: 12),
        ),
      ),
      darkTheme: ThemeData.dark(),
      themeMode: ThemeMode.dark,
      home: HomePage(title: 'KlangDienst'),
      //home: HomePageTest(),
    );
  }
}
