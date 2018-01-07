# Insula

Simple Multi-threaded Fractal Rendering program

![Mandelbrot](doc/mandelbrot.png)

## Usage

Use
```bash
Insula -h
```
to see the list of available command line arguments. These include:

1. `-o`: Output directory. By default it is equal to `pwd`.
2. `-i`: Input file. Mandatory
3. `-t`: Number of Threads. The default (0) is equal to hardware concurrency.
4. `-b`: Bucket size: Chunk size for rendering an image.

The input file has the following format:
```xml
<?xml version="1.0" encoding="utf-8"?>
<scene>
	<parameters>
		<filename>...</filename>
		<type>Type</type>
		<imgWidth>Width</imgWidth>
		<imgHeight>Height</imgHeight>
	</parameters>
	...objects...
</scene>
```
Currently the only available type is `Mandelbrot`. In which case a
`<Mandelbrot>` object must be supplied. It has the following format
```xml
<Mandelbrot>
	<centreX>...</centreX>
	<centreY>...</centreY>
	<radius>...</radius>
	<iterations>...</iterations>
	<escapeRadius>...</escapeRadius>
	<cycles>...</cycles>
</Mandelbrot>
```
Note: All arguments except for `<type>` have default values.

For example of usage, see `doc/examples`.

## Building

It is best to do an out of source build at `build/` directory. Create a
subdirectory `build` and execute
```
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```

## Developing

#### Conventions and scripts

The source code is formatted according to the following astyle
configuration:
```
--style=allman
--align-pointer=type	
--close-templates
--convert-tabs
--indent-preproc-block
--indent-preproc-define
--indent=tab=2
--pad-header
--unpad-paren
```
