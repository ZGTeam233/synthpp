#!/usr/bin/env python3

#
# Created by wsnzg6 on 2026/7/16.
# Copyright(c) 2026 ZGTeam233.
#

"""
MIDI 音符解析器
提取所有音符的音高、开始时间、时长，输出为 JSON
"""

import pretty_midi
from pathlib import Path
from typing import List, Dict, Any
import json


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