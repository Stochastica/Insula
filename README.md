# Insula

Simple Fractal Rendering program

![Mandelbrot](doc/mandelbrot.png)

## Usage

Use
```bash
Insula -h
```
to see the list of available command line arguments.

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
