"""Render a terminal transcript captured from the actual C++ build and run.

No graphs or metrics are simulated. Every output line comes from run.sh or
head reading the CSV files that this invocation generated. Pauses aid reading.
"""
import os
import subprocess
import tempfile
from pathlib import Path

from PIL import Image, ImageDraw, ImageFont

ROOT = Path(__file__).resolve().parents[1]
WIDTH, HEIGHT = 940, 600


def main():
    frames, durations, lines = [], [], []
    font_path = "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf"
    font = ImageFont.truetype(font_path, 17) if Path(font_path).exists() else ImageFont.load_default(size=17)

    def capture(duration=500):
        image = Image.new("RGB", (WIDTH, HEIGHT), "#101820")
        draw = ImageDraw.Draw(image)
        draw.rectangle((0, 0, WIDTH, 42), fill="#21303d")
        draw.text((18, 12), "Graphing Algorithms Project - actual terminal output", font=font, fill="#e7f1fa")
        for index, line in enumerate(lines[-22:]):
            color = "#72e6ad" if line.startswith("$ ") else "#d7e3ed"
            draw.text((20, 60 + index * 22), line, font=font, fill=color)
        draw.text((20, 568), "C++ executable + generated CSV files | pauses added for readability", font=font, fill="#8ba4b5")
        frames.append(image)
        durations.append(duration)

    with tempfile.TemporaryDirectory(prefix="graph-demo-") as output:
        env = dict(os.environ, DEMO_OUTPUT=output)
        commands = [
            './run.sh --output "$DEMO_OUTPUT"',
            'head -n 4 "$DEMO_OUTPUT/er_info.csv" "$DEMO_OUTPUT/ba_info.csv"',
            'head -n 4 "$DEMO_OUTPUT/er_diameter.csv" "$DEMO_OUTPUT/ba_diameter.csv"',
            'head -n 4 "$DEMO_OUTPUT/er_clustering.csv" "$DEMO_OUTPUT/ba_clustering.csv"',
        ]
        for command in commands:
            lines.clear()
            # Wrap only long command labels; the command itself runs unchanged.
            if len(command) > 86:
                first, second = command.split(' "$DEMO_OUTPUT/', 1)
                lines.extend(["$ " + first + " \\", '  "$DEMO_OUTPUT/' + second])
            else:
                lines.append("$ " + command)
            capture(1000)
            process = subprocess.Popen(command, shell=True, executable="/bin/bash", cwd=ROOT,
                                       env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
            for line in process.stdout:
                lines.append(line.rstrip())
                capture(350)
            if process.wait() != 0:
                raise RuntimeError(f"Command failed: {command}")
            durations[-1] += 1700

        import csv
        for model in ("er", "ba"):
            with open(Path(output) / f"{model}_info.csv") as stream:
                rows = list(csv.DictReader(stream))
            assert [int(row["Size"]) for row in rows] == [100, 500, 1000]
            for row in rows:
                size, edges = int(row["Size"]), int(row["Edges"])
                assert 0 < edges <= size * (size - 1) // 2
                with open(Path(output) / f"{size}_{model}_degree.csv") as stream:
                    distribution = list(csv.DictReader(stream))
                assert sum(int(item["Frequency"]) for item in distribution) == size
                assert sum(int(item["Degree"]) * int(item["Frequency"]) for item in distribution) == 2 * edges

    frames[0].save(ROOT / "demo.gif", save_all=True, append_images=frames[1:],
                   duration=durations, loop=0, optimize=True)
    print(f"Captured {len(frames)} actual terminal states; verified CSV sizes and degree/edge consistency.")


if __name__ == "__main__":
    main()
