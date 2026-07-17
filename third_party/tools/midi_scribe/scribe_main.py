#!/usr/bin/env python3

#
# Created by wsnzg6 on 2026/7/16.
# Copyright(c) 2026 ZGTeam233.
#

"""
midi_scribe_tool - MIDI → JSON 解析工具

用法：
    python midi_scribe.py input.mid [output.json] [--no-fix] [--no-name]

示例：
    python midi_scribe.py input.mid out.json
    python midi_scribe.py input.mid  # 自动生成 in.json
    python midi_scribe.py input.mid --no-fix  # 跳过修复
    python midi_scribe.py input.mid out.json --no-name  # 不包含音符名称
"""

import argparse
import sys
from pathlib import Path

from midi_fixer import MidiGlobalEventFixer
from midi_parser import MidiNoteParser


def main():
    parser = argparse.ArgumentParser(description="MIDI → JSON 解析工具")
    parser.add_argument("input", help="输入的 MIDI 文件路径")
    parser.add_argument("output", nargs="?", help="输出的 JSON 文件路径（默认：输入文件名.json）")
    parser.add_argument("--no-fix", action="store_true", help="跳过 MIDI 修复步骤")
    parser.add_argument("--no-name", action="store_true", help="不包含音符名称（仅音高编号）")
    parser.add_argument("--include-drums", action="store_true", help="包含打击乐轨道")

    args = parser.parse_args()

    input_path = Path(args.input)
    if not input_path.exists():
        print(f"文件不存在: {input_path}", file=sys.stderr)
        sys.exit(1)

    # 确定输出路径
    if args.output:
        output_path = Path(args.output)
    else:
        output_path = input_path.with_suffix(".json")

    try:
        # 步骤 1：修复 MIDI（默认开启）
        midi_file = input_path
        if not args.no_fix:
            fixer = MidiGlobalEventFixer(str(input_path))
            fixed_path = input_path.with_suffix(".fixed.mid")
            fixer.save(str(fixed_path))
            midi_file = fixed_path
            print(f"已生成修复文件: {fixed_path}")

        # 步骤 2：解析并导出 JSON
        parser = MidiNoteParser(str(midi_file))
        parser.parse(skip_drums=not args.include_drums)
        parser.to_json(str(output_path), include_note_name=not args.no_name)

        print("完成！")

    except Exception as e:
        print(f"错误: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()