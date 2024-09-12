# bible-verses
A simple [`fortune-mod`](https://github.com/shlomif/fortune-mod/) like command-line utility that outputs a random verse from the Bible. Currently supports:
* King James Version (KJV)
* American Standard-ASV1901 (ASV)

The bible verse data is built using [`bible-databases`](https://github.com/scrollmapper/bible_databases/), a SQL database of bible translations that are in the public domain. I want to include other versions as well but the copyright for modern bible translations makes this difficult.


## Install:
1. `git clone https://github.com/connortynan/bible-verse.git && cd bible-verse`
2. `make && sudo make install && make clean`
3. this will install bible-verse to `/usr/games/` by default, as well as create the directory `/usr/share/games/bible-verses/` where formatted `.txt` files of the bible are stored.

## Usage:
After installing, as long as `/usr/games/` is in `$PATH`, running the command `bible-verse` should just work.
Doing `bible-verse -h` shows the help menu for optional arguments to select specific books to choose from or changing the version.
