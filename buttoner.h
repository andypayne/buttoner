#ifndef BUTTONER_H
#define BUTTONER_H

class Buttoner
{
  private:
    // _val               - the current value of the button
    // _pressedVal        - the value of the button when pressed
    // _unpressedVal      - the value of the button when not pressed
    // _isPressed         - internal value for state tracking
    // _isPressedDown     - triggers once on each press (single, double, or hold)
    // _isSinglePressed   - a single press
    // _isDoublePressed   - a double press
    // _isHeld            - a press and hold
    // _isHeldStarted     - the hold started in this cycle
    // _isReleased        - release for _isPressedDown
    // _isHoldReleased    - the hold was released in this cycle
    // _isSingleReleased  - release for _isSinglePressed
    // _isDoubleReleased  - release for _isDoublePressed

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
    bool _isHoldReleased;
    bool _isSingleReleased;
    bool _isDoubleReleased;

    static const int statusLen = 74;
    char _statusStr[statusLen];

  public:
    // TODO: Make the thresholds optional
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
      _isReleased(false),
      _isHoldReleased(false),
      _isSingleReleased(false),
      _isDoubleReleased(false),
      _statusStr("")
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
    const bool isHoldReleased() const { return _isHoldReleased; }
    const bool isSingleReleased() const { return _isSingleReleased; }
    const bool isDoubleReleased() const { return _isDoubleReleased; }

    void setup() {
      // Future
    }

    void update() {
      long nowTime = millis();
      bool wasPressed = _isPressedDown;
      bool wasSDH = _isHeld || _isSinglePressed || _isDoublePressed || _isHeldStarted || _isReleased || _isSingleReleased || _isDoubleReleased;

      if (_isHeld && _isHeldStarted) {
        _isHeldStarted = false;
      }
      if (_isReleased) {
        _isReleased = false;
      }
      if (_isHoldReleased) {
        _isHoldReleased = false;
      }
      if (_isSingleReleased) {
        _isSingleReleased = false;
      }
      if (_isDoubleReleased) {
        _isDoubleReleased = false;
      }
      if (_isDoublePressed) {
        _isDoublePressed = false;
        // _isDoubleReleased fires right after _isDoublePressed
        _isDoubleReleased = true;
        _isReleased = true;
      }
      if (_isSinglePressed) {
        _isSinglePressed = false;
        // _isSingleReleased fires right after _isSinglePressed
        _isSingleReleased = true;
        _isReleased = true;
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
          _isHoldReleased = false;
          _isPressedDown = false;
        }
        _isPressedUp = false;
      } else if (_val == _unpressedVal) {
        if (!_isPressedUp && wasPressed) {
          _isReleased = true;
        }
        _isPressedUp = true;
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
          _isHoldReleased = true;
          _pressCount = 0;
        } else if (_isPressed && (long)(nowTime - _pressedAt) > _threshMillis) {
          if (!wasSDH && _pressCount == 1) {
            _isHeld = false;
            _isDoublePressed = false;
            _isSinglePressed = true;
          }
          _isPressed = false;
          _pressCount = 0;
        }
      } else {
        // Hmm
      }
    }

    const char *statusToStr() {
      //if (_isPressed ||
      if (_isPressedDown ||
          _isSinglePressed ||
          _isDoublePressed ||
          _isHeldStarted ||
          _isHeld ||
          _isReleased ||
          _isHoldReleased ||
          _isSingleReleased ||
          _isDoubleReleased) {
        //sprintf(_statusStr, "[ ip: %c  pd: %c  sp: %c  dp: %c  hs: %c  ih: %c  ir: %c  hr: %c  sr: %c  dr: %c ]",
        sprintf(_statusStr, "[ pd: %c  sp: %c  dp: %c  hs: %c  ih: %c  ir: %c  hr: %c  sr: %c  dr: %c ]",
          //_isPressed        ? 'X' : ' ',
          _isPressedDown    ? 'X' : ' ',
          _isSinglePressed  ? 'X' : ' ',
          _isDoublePressed  ? 'X' : ' ',
          _isHeldStarted    ? 'X' : ' ',
          _isHeld           ? 'X' : ' ',
          _isReleased       ? 'X' : ' ',
          _isHoldReleased   ? 'X' : ' ',
          _isSingleReleased ? 'X' : ' ',
          _isDoubleReleased ? 'X' : ' ');
      } else {
        memset(_statusStr, 0, sizeof(_statusStr));
      }
      return _statusStr;
    }
};

#endif
