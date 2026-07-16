#
# Created by wsnzg6 on 2026/7/16.
# Copyright(c) 2026 ZGTeam233.
#

from midi_note_parser import *

if __name__ == "__main__":
    try:
        # 1. 创建解析器实例
        parser = MidiNoteParser("in.mid")

        # 2. 解析并保存
        parser.parse_and_save("test.txt", skip_drums=True)

    except FileNotFoundError as e:
        print(f"错误: {e}")
    except Exception as e:
        print(f"解析过程中发生错误: {e}")