#
# Created by wsnzg6 on 2026/7/16.
# Copyright(c) 2026 ZGTeam233.
#

from midi_note_parser import *
from midi_fixer import *

if __name__ == "__main__":
    fixer = MidiGlobalEventFixer("in.mid")

    # 2. 直接保存修复后的文件
    fixer.save("test.mid")

    # 3. 然后正常使用 pretty_midi
    try:
        # 1. 创建解析器实例
        parser = MidiNoteParser("test.mid")

        # 2. 解析并保存
        parser.parse_and_save_json("out.json", include_note_name=True)

    except FileNotFoundError as e:
        print(f"错误: {e}")
    except Exception as e:
        print(f"解析过程中发生错误: {e}")