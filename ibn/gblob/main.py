from typing import Dict, List, Optional
from lark import Lark, Transformer
import sys
import os
import glob

class GBlobTransformer(Transformer):
    def __init__(self, gblob):
        self.gblob = gblob
        super().__init__()
        
    def set_cmd(self, args):
        key, value = args
        self.gblob.set(str(key), str(value))
        
    def append_cmd(self, args):
        list_name, value = args
        self.gblob.append_list(str(list_name), str(value))
        
    def get_cmd(self, args):
        key = str(args[0])
        value = self.gblob.get(key)
        if value:
            print(value)
            
    def list_cmd(self, args):
        list_name = str(args[0])
        values = self.gblob.get_list(list_name)
        for value in values:
            print(value)
            
    def string(self, s):
        return s[0]
        
    def identifier(self, i):
        return i[0]

class GBlobMacro:
    """A declarative macro language for MiSH runtime"""
    
    GRAMMAR = """
    start: command*
    command: set_cmd | append_cmd | get_cmd | list_cmd
    set_cmd: "==>" identifier string -> set_cmd
    append_cmd: "=>" identifier string -> append_cmd
    get_cmd: "<==>" identifier -> get_cmd
    list_cmd: "<=>" identifier -> list_cmd
    identifier: /[a-zA-Z_][a-zA-Z0-9_]*/
    string: /[^\\n]+/
    
    %import common.WS
    %ignore WS
    """
    
    def __init__(self):
        self.variables: Dict[str, str] = {}
        self.lists: Dict[str, List[str]] = {}
        self.parser = Lark(self.GRAMMAR, parser='lalr')
        self.transformer = GBlobTransformer(self)
        
    def set(self, key: str, value: str) -> None:
        """Set a global variable"""
        self.variables[key] = value
        
    def get(self, key: str) -> Optional[str]:
        """Get a global variable value"""
        return self.variables.get(key)
        
    def append_list(self, list_name: str, value: str) -> None:
        """Append value to a global list"""
        if list_name not in self.lists:
            self.lists[list_name] = []
        self.lists[list_name].append(value)
        
    def get_list(self, list_name: str) -> List[str]:
        """Get all values in a global list"""
        return self.lists.get(list_name, [])

    def parse_and_execute(self, code: str) -> None:
        """Parse and execute GBlob code"""
        tree = self.parser.parse(code)
        self.transformer.transform(tree)

def find_g_files(directory: str) -> List[str]:
    """Find all .g files in the given directory"""
    if not os.path.isdir(directory):
        return []
    pattern = os.path.join(directory, "*.g")
    return glob.glob(pattern)

def main():
    """Main entry point for GBlob CLI"""
    if len(sys.argv) < 2:
        print("Usage: gblob <directory>")
        sys.exit(1)
        
    directory = sys.argv[1]
    g_files = find_g_files(directory)
    
    if not g_files:
        print(f"No .g files found in {directory}")
        sys.exit(1)
        
    gblob = GBlobMacro()
    
    for g_file in g_files:
        try:
            with open(g_file, 'r') as f:
                code = f.read()
                gblob.parse_and_execute(code)
        except Exception as e:
            print(f"Error processing {g_file}: {str(e)}")
            
if __name__ == "__main__":
    main()
