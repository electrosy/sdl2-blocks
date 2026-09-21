Ablockalypse font files
=======================

Shipped:
  MartianMono-Regular.ttf   Default UI font (Latin + Cyrillic).
                            Used for en, es, fr, de, pt-BR, ru.

Optional CJK drop-ins (not shipped). Place these exact filenames here
so Japanese and Simplified Chinese can render. The first name in each
pair is preferred (matches the live asset tree); the NotoSansCJK*
names are also accepted:

  NotoSansJP-Regular.otf     Japanese (ja) — preferred
  NotoSansCJKjp-Regular.otf  Japanese (ja) — alternate
  NotoSansSC-Regular.otf     Simplified Chinese (zh-CN) — preferred
  NotoSansCJKsc-Regular.otf  Simplified Chinese (zh-CN) — alternate

If neither file for a locale is present, the game logs a warning and
falls back to MartianMono-Regular.ttf. It will not crash.
