from sys import argv

def main() -> None:
    text: str
    with open(argv[1], "rt") as f:
        text = f.read()
    text = text.replace(r'\t', '\t').replace(r'\n', '\n')
    with open(argv[1], "wt") as f:
        f.write(text)

if __name__ == '__main__':
    main()