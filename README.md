split
=====

This utility splits files into several parts, that's all.

The program will try to open the input file in read-only mode and then split it to multiple 1024 MiB (1073741824 bytes) parts. Output filenames will be the `<output file prefix><part number>`, where `<part number>` is an 8 character long zero padded integer starting from 0, so if the `<output file prefix>` is "part" then the created files will be `part00000000`, `part00000001`, `part00000002` and so on.

The files created are simply the input file split into multiple parts so joining the parts together again is simple as copying more binary files (use /B) with one destination:
```
copy /b part00000000+part00000001+part00000002 joined.txt
```

Note: the input file should exactly match (checksums must be equal) with the joined one. If this is not the case, something went wrong for sure!

The block size is currently hard-coded, might change in the future.

Usage
-----
```
split <input filename> <output file prefix>
```

Example
-------
```
split test.txt part
```

(Yeah, basically I had enough of all the crappy shareware/bloatware/gradient-in-the-background-so-it-is-eight-megabytes-ware "split" solutions for Windows...)
