from hashlib import sha1
import argparse
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Try to guess sha1 plaintext')
    parser.add_argument('file', help='input file of plaintexts to try', type=argparse.FileType('r'))
    parser.add_argument('hash', help='hash to match with', type=str)
    args = parser.parse_args()
    
    for line in args.file:
        stripped = line.rstrip('\n')
        if (sha1(stripped).hexdigest() == args.hash):
            print "sha1 inverse found: " + stripped
            exit()






    
