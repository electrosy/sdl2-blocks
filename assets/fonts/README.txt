Ablockalypse font files
=======================

Shipped:
  MartianMono-Regular.ttf   Default UI font (Latin + Cyrillic).
                            Used for en, es, fr, de, pt-BR, ru.

Optional CJK drop-ins (not committed). The live tree uses NotoSansJP/SC.
Place these exact filenames here so Japanese and Simplified Chinese can
render. NotoSansCJK* names are accepted if the live-tree files are absent:

  NotoSansJP-Regular.otf     Japanese (ja) — live tree
  NotoSansCJKjp-Regular.otf  Japanese (ja) — fallback name
  NotoSansSC-Regular.otf     Simplified Chinese (zh-CN) — live tree
  NotoSansCJKsc-Regular.otf  Simplified Chinese (zh-CN) — fallback name

If neither file for a locale is present, the game logs a warning and
falls back to MartianMono-Regular.ttf. It will not crash.
