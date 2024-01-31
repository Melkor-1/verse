# Verse Program

<a href="https://scan.coverity.com/projects/melkor-1-verse">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/29706/badge.svg"/>
</a>
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/Melkor-1/verse/blob/main/LICENSE)

Verse is a command-line program that allows you to retrieve specific verses from the Quran. It takes a chapter and verse number as input and provides you with the corresponding Quranic verse.

## Dependencies

Before you can use the Verse program, make sure you have the following dependency installed:

- **libcurl**: A library for transferring data with URLs. It is used to download Quranic verses from a remote server.
   - Official Website: [https://curl.se/libcurl/](https://curl.se/libcurl/)
- **jansson**: A C library for encoding, decoding, and manipulating JSON data. It is used for parsing JSON responses from a remote server.
   - Official Website: [https://digip.org/jansson/](https://digip.org/jansson/)

## Building the Program

To build the Verse program, follow these steps:

1. Clone the repository to your local machine:
   ~~~
   git clone https://github.com/Melkor-1/verse.git
   ~~~
   
2. Change into the program's directory:
   ~~~
   cd verse
   ~~~
   
4. Compile the program using the provided Makefile:
   ~~~
   make
   ~~~
   
## Usage

After building the program, you can use it to find Quranic verses by providing a chapter and verse number. Here's the basic usage:

~~~
cd bin
./verse chapter verse
~~~

Replace 'chapter' with the desired chapter number and 'verse' with the desired verse number.

For example, to retrieve chapter 2, verse 255:

~~~
./verse 2 255
~~~

## Contributing

If you'd like to contribute to the Verse Finder program, feel free to fork the repository, make your changes, and submit a pull request. Bug fixes, improvements, and new features are always welcome!


## License
This program is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.


