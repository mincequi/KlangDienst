import 'dart:async';
import 'package:flutter/material.dart';

class HoldIconButton extends StatefulWidget {
  final Icon icon;
  final VoidCallback onPressed;
  final Duration holdDelay;

  const HoldIconButton({
    Key? key,
    required this.icon,
    required this.onPressed,
    this.holdDelay = const Duration(milliseconds: 50),
  }) : super(key: key);

  @override
  _HoldIconButtonState createState() => _HoldIconButtonState();
}

class _HoldIconButtonState extends State<HoldIconButton> {
  Timer? _timer;

  void _startHold() {
    _timer = Timer.periodic(widget.holdDelay, (timer) {
      widget.onPressed(); // Repeatedly call the callback
    });
  }

  void _stopHold() {
    _timer?.cancel();
    _timer = null;
  }

  @override
  Widget build(BuildContext context) {
    return GestureDetector(
      onTapDown: (_) => _startHold(),
      onTapUp: (_) => _stopHold(),
      onTapCancel: _stopHold,
      child: IconButton(
        visualDensity: const VisualDensity(horizontal: -4.0, vertical: 0.0),
        icon: widget.icon,
        onPressed: widget.onPressed, // Required by IconButton but not used
      ),
    );
  }

  @override
  void dispose() {
    _stopHold(); // Cleanup timer on dispose
    super.dispose();
  }
}
