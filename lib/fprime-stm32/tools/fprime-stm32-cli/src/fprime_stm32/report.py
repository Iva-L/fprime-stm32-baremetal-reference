from dataclasses import dataclass, field
from pathlib import Path

from fprime_stm32.memory_model import MemoryMap


@dataclass
class FileChange:
    label: str
    path: Path
    actions: list[str]


@dataclass
class SyncReport:
    chip_tag: str
    memory_map: MemoryMap
    linker_actions: list[str]
    startup_actions: list[str]
    cmake_actions: list[str]
    linker_out: Path
    startup_out: Path
    cmake_out: Path
    dry_run: bool
    project_wiring: list[FileChange] = field(default_factory=list)
    deployment_note: str | None = None
    warnings: list[str] = field(default_factory=list)

    def render(self) -> str:
        def region_line(label: str, region) -> str:
            kib = region.length_bytes / 1024
            return f"  {label:<10} 0x{region.origin:08X}  {kib:.0f} KiB  (region {region.name!r})"

        lines = [
            f"fprime-stm32 sync - STM32H7 target: {self.chip_tag}",
            "",
            "Memory bounds extracted:",
            region_line("FLASH", self.memory_map.flash),
            region_line("AXI SRAM", self.memory_map.axi_sram),
            region_line("DTCM RAM", self.memory_map.dtcm),
            "",
            "Linker script changes:",
            *[f"  - {a}" for a in self.linker_actions],
            "",
            "Startup script changes:",
            *[f"  - {a}" for a in self.startup_actions],
            "",
            "Hardware/CMakeLists.txt changes:",
            *[f"  - {a}" for a in self.cmake_actions],
            "",
        ]

        for change in self.project_wiring:
            lines.append(f"{change.label} changes:")
            if change.actions:
                lines.extend(f"  - {a}" for a in change.actions)
            else:
                lines.append("  - already wired, no changes needed")
            lines.append("")

        if self.deployment_note:
            lines.append(self.deployment_note)
            lines.append("")

        all_warnings = [*self.memory_map.warnings, *self.warnings]
        if all_warnings:
            lines.append("Warnings:")
            lines.extend(f"  ! {w}" for w in all_warnings)
            lines.append("")

        out_files = [self.linker_out, self.startup_out, self.cmake_out, *(c.path for c in self.project_wiring)]
        if self.dry_run:
            lines.append("(dry run - no files were written)")
            lines.extend(f"  would write: {p}" for p in out_files)
        else:
            lines.append("Files written:")
            lines.extend(f"  {p}" for p in out_files)

        return "\n".join(lines)
