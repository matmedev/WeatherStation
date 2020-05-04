# Weather Station
## Get started
To build the code, add a ```credentials.h``` file with the following content:
```
#ifndef CREDENTIALS_H_
#define CREDENTIALS_H_

// Something is needed at the end, because converting it to String chops down the last character
#define WEATHER_API_KEY "[YOUR_API_KEY]"

#endif

```

You can get your own API_KEY by registering at [OpenWeatherMap](https://openweathermap.org/).
