# ArduinoLSM6DS3 library

## Classes

### `GigaDisplayRGB`

Base class for all RGB functions.

#### Syntax

```
GigaDisplayRGB rgb; //creates an object

rgb.begin(); //using the object
```

## Methods

### `begin()`

Initialize the library.

#### Syntax 

```
rgb.begin()
```

#### Parameters

None.

#### Returns

None.

### `on()`

Set pixel color (r,g,b).

#### Syntax 

```
rgb.on(r, g, b)
```

#### Parameters

Accepts values in range of 0-255 (8-bit).

- `uint8_t`, `uint8_t`, `uint8_t` 

#### Returns

None.

### `off()`

Turns off all pixels by setting all parameters to 0. Same effect as using `rgb.on(0,0,0)`

#### Syntax 

```
rgb.off()
```

#### Parameters

None.

#### Returns

None.
