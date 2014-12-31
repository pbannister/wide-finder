wide-finder
===========

This is a simple tool to read sequential files and feed chunks to worker scripts. Particularly useful for processing large log files (or the like).

How can you best take advantage of multiple CPUs when processing a sequential file? The [Wide Finder](https://www.tbray.org/ongoing/When/200x/2008/05/01/Wide-Finder-2) exercise was an attempt to answer that question. (Tim Bray put this up while he was at Sun. Unfortunately, the Sun-hosted pages have gone away.)

Most of the **Wide Finder** submissions were complex and extremely specific to the exact offered problem. The chance to re-using those solutions is low, and the effort required relatively high.

I took a [different approach](http://bannister.us/weblog/2008/wrapping-up-wide-finder-2/). I looked for something simple that could easily be re-used.

The compiled **feed-workers** command line tool reads sequential file(s) in the most efficient (fastest) manner, and throws chunks of lines in round-robin fashion down pipes to the standard input of a fixed number of readers. The readers can be anything (that is presumably more CPU-bound than IO-limited). My offered examples are Perl scripts.

Perl regular expressions are wonderfully flexible, but not especially fast. If you need to do adhoc and/or complex processing on log files (or the like), Perl is a suitable tool. (Likely the same argument applies to Python and Ruby.)

With **feed-workers** reading the input file(s), and feeding one instance of a Perl script per CPU, you can reduce elapsed time by roughly the number of CPUs. If you are processing very large files, you can reduce the time by 4 or 8 times (or more).  (Given the cost of multi-CPU servers versus the cost of programmer time, much of the time it is better to throw hardware at the problem, rather than code longer solutions.)
