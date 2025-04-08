# Ret
An x86-64 ROP gadget search command-line utility.

## Dependencies
- [udis86](https://github.com/vmt/udis86)

## Installation
```bash
git clone https://github.com/jeremykaragania/ret.git
cd ret/ret
make
```

## Usage
```bash
./ret [-b base] [-l length] elffile
```

## Examples
Print ROP gadgets with four instructions.
```bash
./ret -l 4 elffile
 ```
Print sorted ROP gadgets.
```bash
./ret elffile | sort -k 1.17
 ```
Print sorted unique ROP gadgets.
```bash
./ret elffile | sort -k 1.17 | uniq -s 17
```

## License
[MIT](LICENSE)
