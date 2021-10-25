#!/usr/bin/env python3
from urllib import request
import hashlib
import sys
import os
from string import Template
import re

def main(): 
    # default fallback URL -- just for testing
    download_url = "https://github.com/CommunityGD32Cores/ArduinoCore-GD32/archive/9fb26ac.zip"
    if len(sys.argv) == 2:
        download_url = sys.argv[1]
    else:
        print("== WARNING == Using fallback download URL %s, only for local testing." % download_url)
        print("Give this script the download URL as its first argument.")
    # Download the file from `url` and save it locally under `file_name`:
    file_name = os.path.basename(download_url)
    if os.path.isfile(file_name):
        print("Removing old archive %s" % file_name)
        os.remove(file_name)
    print("Saving \"%s\" to %s." % (download_url, file_name))
    github_wanted_filename = ""
    try:
        _, msg = request.urlretrieve(download_url, file_name)
        print("Extracting GitHub returned filename from Content-Disposition header.")
        content_disp = msg.get("content-disposition") 
        github_wanted_filename = re.findall("filename=(.+)", content_disp)[0]
        print("Extracted: %s" % github_wanted_filename)
    except Exception as exc:
        print("Downloading failed!")
        print(exc)
        exit(-1)
    file_size = os.path.getsize(file_name)
    print("Download done, file size is %d bytes (%.2f MB)" % (file_size, file_size / 1024.0 / 1024.0))
    # Generate SHA256 hash (as is the recommended hash algorithm after SHA1)
    sha256_hash = ""
    try:
        hash_state = hashlib.sha256()
        with open(file_name,"rb") as f:
            # Read and update hash string value in blocks of 1MB
            for byte_block in iter(lambda: f.read(1*1024*1024),b""):
                hash_state.update(byte_block)
            sha256_hash = hash_state.hexdigest()
    except Exception as exc:
        print("Generating SHA256 hash failed!")
        print(exc)
        exit(-1)
    print("SHA256 of \"%s\": %s" % (file_name, sha256_hash))
    # Generate index JSON file
    template_file = "package_gd32_index.json.tpl"
    index_out_file = "package_gd32_index.json"
    content = ""
    try:
        with open(template_file) as fp:
            data = Template(fp.read())
            content = data.substitute(
                url=download_url,
                archiveFilename=github_wanted_filename,
                size=str(file_size),
                checksum="SHA-256:%s" % sha256_hash
            )
        with open(index_out_file, 'wb') as the_file:
            the_file.write(content.encode('utf-8'))
        print("Index file %s successfully written!" % index_out_file)
    except Exception as exc:
        print("Generating %d failed" % index_out_file)
        print(exc)
        exit(-1)
    
if __name__ == '__main__':
    main()