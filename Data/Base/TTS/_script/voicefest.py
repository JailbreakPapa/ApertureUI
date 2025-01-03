#!/usr/bin/env python3
import hashlib
import json
import unittest
from dataclasses import dataclass
from pathlib import Path
from typing import Set

_DIR = Path(__file__).parent
_REPO_DIR = _DIR.parent


@dataclass
class Language:
    native: str
    english: str
    country: str


_LANGUAGES = {
    "ar_JO": Language("العربية", "Arabic", "Jordan"),
    "ca_ES": Language("Català", "Catalan", "Spain"),
    "cs_CZ": Language("Čeština", "Czech", "Czech Republic"),
    "cy_GB": Language("Cymraeg", "Welsh", "Great Britain"),
    "da_DK": Language("Dansk", "Danish", "Denmark"),
    "de_DE": Language("Deutsch", "German", "Germany"),
    "el_GR": Language("Ελληνικά", "Greek", "Greece"),
    "en_GB": Language("English", "English", "Great Britain"),
    "en_US": Language("English", "English", "United States"),
    "es_ES": Language("Español", "Spanish", "Spain"),
    "es_MX": Language("Español", "Spanish", "Mexico"),
    "fa_IR": Language("فارسی", "Farsi", "Iran"),
    "fi_FI": Language("Suomi", "Finnish", "Finland"),
    "fr_FR": Language("Français", "French", "France"),
    "is_IS": Language("íslenska", "Icelandic", "Iceland"),
    "it_IT": Language("Italiano", "Italian", "Italy"),
    "hu_HU": Language("Magyar", "Hungarian", "Hungary"),
    "ka_GE": Language("ქართული ენა", "Georgian", "Georgia"),
    "kk_KZ": Language("қазақша", "Kazakh", "Kazakhstan"),
    "lb_LU": Language("Lëtzebuergesch", "Luxembourgish", "Luxembourg"),
    "lv_LV": Language("Latviešu", "Latvian", "Latvia"),
    "ne_NP": Language("नेपाली", "Nepali", "Nepal"),
    "nl_BE": Language("Nederlands", "Dutch", "Belgium"),
    "nl_NL": Language("Nederlands", "Dutch", "Netherlands"),
    "no_NO": Language("Norsk", "Norwegian", "Norway"),
    "pl_PL": Language("Polski", "Polish", "Poland"),
    "pt_BR": Language("Português", "Portuguese", "Brazil"),
    "pt_PT": Language("Português", "Portuguese", "Portugal"),
    "ro_RO": Language("Română", "Romanian", "Romania"),
    "ru_RU": Language("Русский", "Russian", "Russia"),
    "sk_SK": Language("Slovenčina", "Slovak", "Slovakia"),
    "sl_SI": Language("Slovenščina", "Slovenian", "Slovenia"),
    "sr_RS": Language("srpski", "Serbian", "Serbia"),
    "sv_SE": Language("Svenska", "Swedish", "Sweden"),
    "sw_CD": Language("Kiswahili", "Swahili", "Democratic Republic of the Congo"),
    "tr_TR": Language("Türkçe", "Turkish", "Turkey"),
    "uk_UA": Language("украї́нська мо́ва", "Ukrainian", "Ukraine"),
    "vi_VN": Language("Tiếng Việt", "Vietnamese", "Vietnam"),
    "zh_CN": Language("简体中文", "Chinese", "China"),
}

# -----------------------------------------------------------------------------


def add_languages():
    for onnx_path in _REPO_DIR.rglob("*.onnx"):
        config_path = f"{onnx_path}.json"
        with open(config_path, "r", encoding="utf-8") as config_file:
            config = json.load(config_file)

        lang_code, dataset, quality = onnx_path.stem.split("-")
        is_changed = False

        lang_info = _LANGUAGES.get(lang_code)
        assert lang_info is not None, f"Missing name for language: {lang_code}"

        lang_family, lang_region = lang_code.split("_", maxsplit=1)
        lang_dict = {
            "code": lang_code,
            "family": lang_family,
            "region": lang_region,
            "name_native": lang_info.native,
            "name_english": lang_info.english,
            "country_english": lang_info.country,
        }

        if "language" not in config:
            config["language"] = lang_dict
            is_changed = True
        else:
            current_lang_dict = config["language"]
            if "code" not in current_lang_dict:
                current_lang_dict["code"] = lang_dict["code"]
                is_changed = True

            if "family" not in current_lang_dict:
                current_lang_dict["family"] = lang_dict["family"]
                is_changed = True

            if "region" not in current_lang_dict:
                current_lang_dict["region"] = lang_dict["region"]
                is_changed = True

            if "name_native" not in current_lang_dict:
                current_lang_dict["name_native"] = lang_dict["name_native"]
                is_changed = True

            if "name_english" not in current_lang_dict:
                current_lang_dict["name_english"] = lang_dict["name_english"]
                is_changed = True

            if "country_english" not in current_lang_dict:
                current_lang_dict["country_english"] = lang_dict["country_english"]
                is_changed = True

        if "dataset" not in config:
            config["dataset"] = dataset
            is_changed = True

        if "quality" not in config["audio"]:
            config["audio"]["quality"] = quality
            is_changed = True

        if is_changed:
            with open(config_path, "w", encoding="utf-8") as config_file:
                json.dump(config, config_file, ensure_ascii=False, indent=2)


# -----------------------------------------------------------------------------


class VoiceTest(unittest.TestCase):
    def test_voices(self):
        used_aliases: Set[str] = set()

        for onnx_path in _REPO_DIR.rglob("*.onnx"):
            with self.subTest(onnx_path=onnx_path):
                self.assertGreater(onnx_path.stat().st_size, 0, "Empty onnx file")

                # Load JSON config for voice
                config_path = onnx_path.parent / f"{onnx_path.name}.json"
                with open(config_path, "r", encoding="utf-8") as config_file:
                    config = json.load(config_file)

                # Verify config
                self.assertIn(
                    "piper_version", config, "Missing piper_version in config"
                )
                self.assertIn("language", config, "Missing language in config")
                self.assertIn("dataset", config, "Missing dataset in config")
                self.assertIn(
                    "quality", config["audio"], "Missing audio quality in config"
                )

                # Verify directory structure
                # <lang_family>/<lang_code>/<dataset>/<quality>/
                quality_dir = onnx_path.parent
                dataset_dir = quality_dir.parent
                lang_code_dir = dataset_dir.parent
                lang_family_dir = lang_code_dir.parent

                self.assertEqual(
                    lang_family_dir.name,
                    config["language"]["family"],
                    "Wrong lang family dir",
                )
                self.assertEqual(
                    lang_code_dir.name,
                    config["language"]["code"],
                    "Wrong lang code dir",
                )
                self.assertEqual(
                    dataset_dir.name, config["dataset"], "Wrong dataset dir"
                )
                self.assertEqual(
                    quality_dir.name, config["audio"]["quality"], "Wrong quality dir"
                )
                self.assertIn(lang_code_dir.name, _LANGUAGES, "Unknown language code")

                # Verify file names
                file_lang_code, file_dataset, file_quality = onnx_path.stem.split("-")
                file_lang_family = file_lang_code.split("_", maxsplit=1)[0]

                self.assertEqual(
                    file_lang_family,
                    config["language"]["family"],
                    "Wrong lang family file",
                )
                self.assertEqual(
                    file_lang_code, config["language"]["code"], "Wrong lang code file"
                )
                self.assertEqual(file_dataset, config["dataset"], "Wrong dataset file")
                self.assertEqual(
                    file_quality, config["audio"]["quality"], "Wrong quality"
                )

                # Verify aliases are unique
                aliases_path = onnx_path.parent / "ALIASES"
                if aliases_path.exists():
                    with open(aliases_path, "r", encoding="utf-8") as aliases_file:
                        for alias in aliases_file:
                            alias = alias.strip()
                            self.assertNotIn(
                                alias,
                                used_aliases,
                                "Alias is already in use by another voice",
                            )
                            used_aliases.add(alias)


def run_tests() -> None:
    runner = unittest.TextTestRunner()
    result = runner.run(unittest.makeSuite(VoiceTest))
    assert not result.failures, "Test failures"


# -----------------------------------------------------------------------------


def write_voices_json() -> None:
    # {
    #   "<family>_<region>-<dataset>-<quality>": {
    #     "key": "<voice_key>",
    #     "name": "<dataset>",
    #     "language": {
    #       "code": "<family>_<region>",
    #       "family": "<family>",
    #       "region": "<region>",
    #       "name_native": "<native>",
    #       "name_english": "<english>",
    #       "country_english": "<country>",
    #     },
    #     "quality": "<quality>",  // x_low, low, medium, high
    #     "num_speakers": int,
    #     "speaker_id_map": {
    #       "name": int,
    #       ...
    #     }
    #     "files": {
    #       "relative/path/to/file": {
    #         "size_bytes": int,
    #         "md5_digest": str,        // hex
    #       },
    #       ...
    #     },
    #     "aliases": ["alias", ...],
    #   },
    #   ...
    # }
    voices = {}

    for onnx_path in sorted(_REPO_DIR.rglob("*.onnx")):
        voice_dir = onnx_path.parent
        config_path = voice_dir / f"{onnx_path.name}.json"
        with open(config_path, "r", encoding="utf-8") as config_file:
            config = json.load(config_file)

        quality = config["audio"]["quality"]
        dataset = config["dataset"]
        lang_code = config["language"]["code"]
        lang_family, lang_region = lang_code.split("_", maxsplit=1)
        lang_names = _LANGUAGES[lang_code]
        voice_key = f"{lang_code}-{dataset}-{quality}"

        model_card_path = voice_dir / "MODEL_CARD"
        assert model_card_path.exists(), f"Missing {model_card_path}"

        aliases: Set[str] = set()
        aliases_path = voice_dir / "ALIASES"
        if aliases_path.exists():
            with open(aliases_path, "r", encoding="utf-8") as aliases_file:
                for alias in aliases_file:
                    aliases.add(alias.strip())

        voices[voice_key] = {
            "key": voice_key,
            "name": dataset,
            "language": {
                "code": lang_code,
                "family": lang_family,
                "region": lang_region,
                "name_native": lang_names.native,
                "name_english": lang_names.english,
                "country_english": lang_names.country,
            },
            "quality": quality,
            "num_speakers": config["num_speakers"],
            "speaker_id_map": config.get("speaker_id_map", {}),
            "files": {
                str(file_path.relative_to(_REPO_DIR)): {
                    "size_bytes": file_path.stat().st_size,
                    "md5_digest": get_file_hash(file_path),
                }
                for file_path in (
                    onnx_path,
                    config_path,
                    model_card_path,
                )
            },
            "aliases": sorted(list(aliases)),
        }

    with open(_REPO_DIR / "voices.json", "w", encoding="utf-8") as voices_file:
        json.dump(voices, voices_file, indent=4, ensure_ascii=False)


def get_file_hash(path, bytes_per_chunk: int = 8192) -> str:
    """Hash a file in chunks using md5."""
    path_hash = hashlib.md5()
    with open(path, "rb") as path_file:
        chunk = path_file.read(bytes_per_chunk)
        while chunk:
            path_hash.update(chunk)
            chunk = path_file.read(bytes_per_chunk)

    return path_hash.hexdigest()


# -----------------------------------------------------------------------------

if __name__ == "__main__":
    add_languages()
    run_tests()

    print("Writing voices.json")
    write_voices_json()
