import os
import re
import requests

# Constants
LOCAL_VOICES_MARKDOWN = "VOICES.md"
DESTINATION_FOLDER = "TTS"

def main():
    print("Starting download of Piper ONNX files...")

    # Check if VOICES.md exists
    if not os.path.exists(LOCAL_VOICES_MARKDOWN):
        print(f"Markdown file not found: {LOCAL_VOICES_MARKDOWN}")
        return

    # Read markdown content
    with open(LOCAL_VOICES_MARKDOWN, "r", encoding="utf-8") as f:
        markdown_content = f.read()

    # Extract file URLs
    file_urls = extract_file_urls(markdown_content)

    if not file_urls:
        print("No file URLs found in VOICES.md.")
        return

    # Download files
    for file_url in file_urls:
        file_name = os.path.basename(file_url)
        language_code = get_language_code(file_name)

        if not language_code:
            print(f"Skipping file with unknown language format: {file_name}")
            continue

        language_directory = os.path.join(DESTINATION_FOLDER, language_code)
        os.makedirs(language_directory, exist_ok=True)

        destination_path = os.path.join(language_directory, file_name)

        if not os.path.exists(destination_path):
            download_file(file_url, destination_path)
        else:
            print(f"File already exists: {destination_path}")

    print("Download complete.")

def download_file(url, destination_path):
    try:
        response = requests.get(url, stream=True)
        response.raise_for_status()

        with open(destination_path, "wb") as f:
            for chunk in response.iter_content(chunk_size=8192):
                f.write(chunk)

        print(f"Downloaded: {destination_path}")
    except requests.RequestException as e:
        print(f"Failed to download {url}: {e}")

def extract_file_urls(markdown_content):
    matches = re.findall(r"\((https://.*?\.(onnx|onnx\.json))\)", markdown_content)
    return [match[0] for match in matches]

def get_language_code(file_name):
    match = re.match(r"^(\w+_\w+)-", file_name)
    return match.group(1) if match else ""

if __name__ == "__main__":
    main()
