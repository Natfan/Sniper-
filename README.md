# Sniper++

## A minecraft name sniper in C++.

# Installation

Sniper++ uses C11.

For libraries, Sniper++ utilizes:

- Gumbo
- Libcurl
- RapidJSON

To run Sniper++, please edit the following variables in the source code:

- Username
- Email
- Password
- Desired Username

And make sure to have the time set.

  t.tm_year = 117;
  t.tm_mon = 11;
  t.tm_mday = 5;
  t.tm_hour = 20;
  t.tm_min = 4;
  t.tm_sec = 8;

This translates to: 21:04:08 12/5/2017

Compile the program using these parameters: g++ sniper.cpp -o xd -l curl -std=c++11 `pkg-config --cflags --libs gumbo`

# What to do:

- Add multithreading therefore fire multiple requests
