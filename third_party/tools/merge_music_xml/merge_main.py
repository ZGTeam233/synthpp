#!/usr/bin/env python3

#
# Created by wsnzg6 on 2026/7/17.
# Copyright(c) 2026 ZGTeam233.
#

from xml_merger import *

def main():
    # 使用链式调用，清晰优雅
    try:
        merger = MusicXMLMerger()
        merger.add_page("page1.mxl") \
            .add_page("page2.mxl") \
            .execute("final_score.mxl")
    except Exception as e:
        print(f"合并过程中出现错误: {e}")

if __name__ == "__main__":
    main()