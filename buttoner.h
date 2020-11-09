#ifndef BUTTONER_H
#define BUTTONER_H

class Buttoner
{
  private:
    // _val             - the current value of the button
    // _pressedVal      - the value of the button when pressed
    // _unpressedVal    - the value of the button when not pressed
    // _isPressed       - internal value for state tracking
    // _isPressedDown   - triggers once on each press (single, double, or hold)
    // _isSinglePressed - a single press
    // _isDoublePressed - a double press
    // _isHeld          - a press and hold
    // _isHeldStarted   - the hold started in this cycle
    // _isReleased      - the hold (or press)  was released in this cycle
    long _val;
    long _pressedVal;
    long _unpressedVal;
    long _threshMillis;
    long _doubleThreshMillis;
    bool _isPressed;
    bool _pressCount;
    long _pressedAt;
    bool _isPressedDown;
    bool _isPressedUp;
    bool _isSinglePressed;
    bool _isDoublePressed;
    bool _isHeld;
    bool _isHeldStarted;
    bool _isReleased;

  public:
    Buttoner(const long initialVal,
             const long pressedVal,
             const long unpressedVal,
             const long threshMillis,
             const long doubleThreshMillis) :
      _val(initialVal),
      _pressedVal(pressedVal),
      _unpressedVal(unpressedVal),
      _threshMillis(threshMillis),
      _doubleThreshMillis(doubleThreshMillis),
      _isPressed(false),
      _pressCount(0),
      _pressedAt(0),
      _isPressedDown(false),
      _isPressedUp(false),
      _isSinglePressed(false),
      _isDoublePressed(false),
      _isHeld(false),
      _isHeldStarted(false),
      _isReleased(false)
    { }

    // setVal - Set the value of the button. Return the previous value.
    const long setVal(const long val) {
      long oldVal = _val;
      _val = val;
      return oldVal;
    }

    const bool isPressedDown() const { return _isPressedDown; }
    const bool isSinglePressed() const { return _isSinglePressed; }
    const bool isDoublePressed() const { return _isDoublePressed; }
    const bool isHeld() const { return _isHeld; }
    const bool isHeldStarted() const { return _isHeldStarted; }
    const bool isReleased() const { return _isReleased; }

    void setup() {
      // Future
    }

    void update() {
      long nowTime = millis();
      bool wasPressed = _isPressedDown;

      if (_isHeld && _isHeldStarted) {
        _isHeldStarted = false;
      }
      if (_isReleased) {
        _isReleased = false;
      }
      if (_isDoublePressed) {
        _isDoublePressed = false;
      }
      if (_isSinglePressed) {
        _isSinglePressed = false;
      }
      if (_isPressedDown) {
        _isPressedDown = false;
      }

      if (_val == _pressedVal) {
        if (wasPressed) {
          _isPressedDown = false;
        } else if (_isPressedUp) {
          _isPressedDown = true;
        }
        if (!_isPressed && _pressCount == 0 && (long)(nowTime - _pressedAt) >= _threshMillis) {
          // Pressed
          _pressCount = 1;
          _isPressed = true;
          _pressedAt = nowTime;
        } else if (!_isPressed && _pressCount == 1 && (long)(nowTime - _pressedAt) >= _doubleThreshMillis) {
          _pressCount = 0;
          _isHeld = false;
          _isSinglePressed = false;
          _isDoublePressed = true;
          _isPressed = true;
          _pressedAt = nowTime;
        } else if (!_isHeld && _isPressed && _pressCount == 1 && (long)(nowTime - _pressedAt) > _threshMillis) {
          _isDoublePressed = false;
          _isSinglePressed = false;
          _isHeld = true;
          _isHeldStarted = true;
          _isPressedDown = false;
        }
        _isPressedUp = false;
      } else if (_val == _unpressedVal) {
        if (!_isPressedUp) {
          _isReleased = true;
        }
        _isPressedUp = true;
        if (_isSinglePressed) {
          _isSinglePressed = false;
          _isPressed = false;
          _isReleased = true;
        }
        if (_isDoublePressed) {
          _isDoublePressed = false;
          _isPressed = false;
          _pressCount = 0;
          _isReleased = true;
        }

        if (_isPressed && _pressCount == 1 && (long)(nowTime - _pressedAt) < _doubleThreshMillis) {
          // Release state for a potential double press
          _isPressed = false;
        } else if (!_isPressed && _pressCount == 1 && (long)(nowTime - _pressedAt) >= _threshMillis) {
          // Single
          _isHeld = false;
          _isDoublePressed = false;
          _isSinglePressed = true;
          _pressCount = 0;
        } else if (_isHeld && _isPressed && _pressCount == 1 && (long)(nowTime - _pressedAt) >= _threshMillis) {
          // Hold release
          _isPressed = false;
          _isHeld = false;
          _isDoublePressed = false;
          _isSinglePressed = false;
          _isReleased = true;
          _pressCount = 0;
        } else if (_isPressed && (long)(nowTime - _pressedAt) > _threshMillis) {
          _isPressed = false;
          _pressCount = 0;
        }
      } else {
        // Hmm
      }
    }
};

#endif
