# 4chanlog

4chanlog - get all media links from the 4chan catalog

## Building on Linux

### Prerequisites
 - `libcurl`
 - `json-c`
 - `gcc`, `clang`, or another C compiler

### Build instruction

`4chanlog` github repo: https://github.com/xfgusta/4chanlog

```
$ make
```

## Examples

Use `head` to get the very first 10 links (and width and height as well)

```
$ ./4chanlog a | head
https://i.4cdn.org/a/1611687714218.png (1400x720)
https://i.4cdn.org/a/1611684376166.png (1328x837)
https://i.4cdn.org/a/1611710223323.png (852x831)
https://i.4cdn.org/a/1611688572006.png (1052x1867)
https://i.4cdn.org/a/1611677138972.jpg (1280x720)
https://i.4cdn.org/a/1611693991934.jpg (258x385)
https://i.4cdn.org/a/1611709538333.jpg (587x501)
https://i.4cdn.org/a/1611710210208.png (713x440)
https://i.4cdn.org/a/1611701248409.png (388x505)
https://i.4cdn.org/a/1611705801081.jpg (800x676)
```

Use `cut` to get the links (without width and height)

```
$ ./4chanlog a | head | cut -d ' ' -f 1
https://i.4cdn.org/a/1611684376166.png
https://i.4cdn.org/a/1611709084440.jpg
https://i.4cdn.org/a/1611709952497.jpg
https://i.4cdn.org/a/1611710040273.png
https://i.4cdn.org/a/1611688883147.png
https://i.4cdn.org/a/1611688572006.png
https://i.4cdn.org/a/1611649606360.jpg
https://i.4cdn.org/a/1611709438829.jpg
https://i.4cdn.org/a/1611701800484.jpg
https://i.4cdn.org/a/1611710073405.jpg
```

Use `cut`, `xargs` and `wget` to save the files 

```
$ ./4chanlog a | cut -d ' ' -f 1 | xargs wget
...
[output suppressed]
```
