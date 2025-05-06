cdef extern from "unistd.h":
    int access(const char* path, int mode)
    int F_OK
    int R_OK
    int W_OK
    int X_OK

cdef extern from "glob.h":
    ctypedef struct glob_t:
        size_t gl_pathc
        char** gl_pathv
        size_t gl_offs
    
    int glob(const char* pattern, int flags, void* errfunc, glob_t* pglob)
    void globfree(glob_t* pglob)
    
    int GLOB_ERR
    int GLOB_MARK 
    int GLOB_NOSORT
    int GLOB_NOCHECK
    int GLOB_NOESCAPE
    int GLOB_BRACE

def check_file_exists(path):
    """
    Check if a file exists
    
    Args:
        path (str): Path to check
        
    Returns:
        bool: True if file exists, False otherwise
    """
    path_bytes = path.encode('utf-8')
    return access(path_bytes, F_OK) == 0

def expand_glob_pattern(pattern):
    """
    Expand a glob pattern to matching files
    
    Args:
        pattern (str): Glob pattern to expand
        
    Returns:
        list: List of matching file paths
    """
    cdef glob_t globbuf
    pattern_bytes = pattern.encode('utf-8')
    
    # Initialize glob structure
    result = []
    ret = glob(pattern_bytes, GLOB_BRACE | GLOB_MARK, NULL, &globbuf)
    
    if ret == 0:
        # Extract matches
        for i in range(globbuf.gl_pathc):
            path = (<bytes>globbuf.gl_pathv[i]).decode('utf-8')
            result.append(path)
            
        # Free allocated memory
        globfree(&globbuf)
        
    return result

def find_g_files(directory="."):
    """
    Find all .g files in a directory
    
    Args:
        directory (str): Directory to search in, defaults to current directory
        
    Returns:
        list: List of .g files found
    """
    if directory.endswith("/"):
        pattern = directory + "*.g"
    else:
        pattern = directory + "/*.g"
    return expand_glob_pattern(pattern)
