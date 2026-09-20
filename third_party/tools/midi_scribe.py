#!/usr/bin/env python3

#
# Created by wsnzg6 on 2026/7/16.
# Copyright(c) 2026 ZGTeam233.
#

import mido
from mido import MidiFile, MidiTrack
from pathlib import Path
from typing import List, Tuple
import pretty_midi
from typing import List, Dict, Any
import json
import argparse
import sys

class MidiGlobalEventFixer:
    """修复 MIDI 文件：将全局元事件移到 Track 0"""

    GLOBAL_TYPES = {
        'set_tempo', 'key_signature', 'time_signature',
        'smpte_offset', 'sequencer_specific'
    }

    def __init__(self, input_path: str):
        self.input_path = Path(input_path)
        if not self.input_path.exists():
            raise FileNotFoundError(f"文件不存在: {self.input_path}")
        self.mid = MidiFile(str(self.input_path))
        self.ticks_per_beat = self.mid.ticks_per_beat

    def fix(self) -> MidiFile:
        """返回修复后的 MidiFile 对象"""
        track_events: List[List[Tuple[int, mido.Message]]] = []
        global_events: List[Tuple[int, mido.Message]] = []

        for track in self.mid.tracks:
            abs_time = 0
            events = []
            for msg in track:
                abs_time += msg.time
                if msg.type in self.GLOBAL_TYPES:
                    global_events.append((abs_time, msg))
                else:
                    events.append((abs_time, msg))
            track_events.append(events)

        # 合并全局事件到第一个轨道
        first_events = track_events[0] if track_events else []
        merged_first = first_events + global_events
        merged_first.sort(key=lambda x: x[0])

        # 重建轨道
        new_tracks = []
        for track_ev in [merged_first] + track_events[1:]:
            track = MidiTrack()
            prev_time = 0
            for abs_time, msg in track_ev:
                delta = abs_time - prev_time
                track.append(msg.copy(time=delta))
                prev_time = abs_time
            new_tracks.append(track)

        return MidiFile(ticks_per_beat=self.ticks_per_beat, tracks=new_tracks)

    def save(self, output_path: str) -> None:
        fixed = self.fix()
        fixed.save(output_path)
        print(f"修复完成: {output_path}")

class MidiNoteParser:
    """解析 MIDI 文件，提取音符信息"""

    def __init__(self, midi_path: str):
        self.midi_path = Path(midi_path)
        if not self.midi_path.exists():
            raise FileNotFoundError(f"MIDI 文件不存在: {self.midi_path}")
        self.midi_data = pretty_midi.PrettyMIDI(str(self.midi_path))
        self.notes: List[Dict[str, Any]] = []

    def parse(self, skip_drums: bool = True) -> List[Dict[str, Any]]:
        """提取所有音符，按时间排序"""
        self.notes.clear()

        for track_idx, instrument in enumerate(self.midi_data.instruments):
            if skip_drums and instrument.is_drum:
                continue
            for note in instrument.notes:
                self.notes.append({
                    'pitch': note.pitch,
                    'start_ms': int(note.start * 1000 + 0.5),
                    'duration_ms': int((note.end - note.start) * 1000 + 0.5),
                    'track': track_idx,
                })

        # 按开始时间排序（跨轨道混合）
        self.notes.sort(key=lambda x: x['start_ms'])
        return self.notes

    def to_json(self, output_path: str, indent: int = 4, include_note_name: bool = True) -> None:
        """导出为 JSON"""
        if not self.notes:
            raise RuntimeError("没有音符数据，请先调用 parse()")

        data = []
        for note in self.notes:
            item = {
                'pitch': note['pitch'],
                'start_ms': note['start_ms'],
                'duration_ms': note['duration_ms'],
            }
            if include_note_name:
                item['note_name'] = pretty_midi.note_number_to_name(note['pitch'])
            data.append(item)

        with open(output_path, 'w', encoding='utf-8') as f:
            json.dump(data, f, indent=indent, ensure_ascii=False)

        print(f"成功写入 {len(data)} 条音符信息到: {output_path}")

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