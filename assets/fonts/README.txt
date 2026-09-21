Ablockalypse font files
=======================

Shipped:
  MartianMono-Regular.ttf   Default UI font (Latin + Cyrillic).
                            Used for en, es, fr, de, pt-BR, ru.

Optional CJK drop-ins (not committed). Place these exact filenames here
so Japanese and Simplified Chinese can render. Prefer the NotoSansCJK*
names; NotoSansJP/SC mirrors are also accepted:

  NotoSansCJKjp-Regular.otf  Japanese (ja) — preferred
  NotoSansJP-Regular.otf     Japanese (ja) — mirror
  NotoSansCJKsc-Regular.otf  Simplified Chinese (zh-CN) — preferred
  NotoSansSC-Regular.otf     Simplified Chinese (zh-CN) — mirror

If neither file for a locale is present, the game logs a warning and
falls back to MartianMono-Regular.ttf. It will not crash.
