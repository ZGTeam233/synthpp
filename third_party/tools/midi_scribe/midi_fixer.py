#!/usr/bin/env python3

#
# Created by wsnzg6 on 2026/7/16.
# Copyright(c) 2026 ZGTeam233.
#

import mido
from mido import MidiFile, MidiTrack
from pathlib import Path
from typing import List, Tuple

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