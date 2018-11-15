# -*- coding: utf-8 -*-
"""
@author: konantech
"""
import re

def regex_search(pattern, string, groups=False, group=None, flags=0):

    regex = re.compile(pattern, flags)
    results = regex.search(string)
    if not results:
        return ''
    else:
        if groups:
            return results.groups()
        elif group is not None:
            return results.group(group)
        else:
            return results
