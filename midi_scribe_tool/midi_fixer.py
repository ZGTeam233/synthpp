#
# Created by wsnzg6 on 2026/7/16.
# Copyright(c) 2026 ZGTeam233.
#

import mido
from mido import MidiFile, MidiTrack
from pathlib import Path

class MidiGlobalEventFixer:
    """
    修复 MIDI 文件：将所有全局元事件（Tempo、Key、Time Signature等）
    移动到第一个轨道（Track 0），消除 pretty_midi 警告。
    """

    # 需要移动到 Track 0 的全局事件类型
    GLOBAL_TYPES = {
        'set_tempo', 'key_signature', 'time_signature',
        'smpte_offset', 'sequencer_specific'
    }

    def __init__(self, input_path: str):
        """
        初始化修复器。

        Args:
            input_path (str): 待修复的 MIDI 文件路径。
        """
        self.input_path = Path(input_path)
        if not self.input_path.exists():
            raise FileNotFoundError(f"文件不存在: {self.input_path}")
        self.mid = MidiFile(str(self.input_path))
        self.tick_ratio = self.mid.ticks_per_beat

    def fix(self) -> MidiFile:
        """
        执行修复逻辑，返回一个新的 MidiFile 对象（内存中）。

        Returns:
            MidiFile: 修复后的 MidiFile 对象。
        """
        # 1. 解析所有轨道，分离全局事件和普通事件（绝对时间）
        track_events = []   # 每个轨道的事件列表 [(abs_time, msg), ...]
        global_events = []  # 所有全局事件 [(abs_time, msg), ...]

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

        # 2. 将全局事件合并到第一个轨道
        first_events = track_events[0] if track_events else []
        merged_first = first_events + global_events
        merged_first.sort(key=lambda x: x[0])  # 按绝对时间排序

        # 3. 构建新轨道（将绝对时间转为 delta time）
        new_tracks = []
        for idx, track_ev in enumerate([merged_first] + track_events[1:]):
            track = MidiTrack()
            prev_time = 0
            for abs_time, msg in track_ev:
                delta = abs_time - prev_time
                track.append(msg.copy(time=delta))
                prev_time = abs_time
            new_tracks.append(track)

        # 4. 返回新的 MidiFile
        return MidiFile(ticks_per_beat=self.tick_ratio, tracks=new_tracks)

    def save(self, output_path: str) -> None:
        """
        修复后保存到文件。

        Args:
            output_path (str): 输出文件路径。
        """
        fixed = self.fix()
        fixed.save(output_path)
        print(f"✅ 修复完成，已保存至: {output_path}")