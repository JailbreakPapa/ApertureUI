using System;
using System.IO;
using System.Net.Http;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

class PiperDownloader
{
  private const string LocalVoicesMarkdown = "VOICES.md";
  private const string DestinationFolder = "D:\\ApertureUI\\Data\\Base\\TTS";

  static async Task Main(string[] args)
  {
    Console.WriteLine("Starting download of Piper ONNX files...");

    try
    {
      if (!File.Exists(LocalVoicesMarkdown))
      {
        Console.WriteLine($"Markdown file not found: {LocalVoicesMarkdown}");
        return;
      }

      string markdownContent = File.ReadAllText(LocalVoicesMarkdown);

      var fileUrls = ExtractFileUrls(markdownContent);

      if (fileUrls.Length == 0)
      {
        Console.WriteLine("No file URLs found in VOICES.md.");
        return;
      }

      using HttpClient client = new HttpClient();

      foreach (var fileUrl in fileUrls)
      {
        string fileName = Path.GetFileName(fileUrl);
        string languageCode = GetLanguageCode(fileName);

        if (string.IsNullOrEmpty(languageCode))
        {
          Console.WriteLine($"Skipping file with unknown language format: {fileName}");
          continue;
        }

        string languageDirectory = Path.Combine(DestinationFolder, languageCode);
        Directory.CreateDirectory(languageDirectory);

        string destinationPath = Path.Combine(languageDirectory, fileName);

        if (!File.Exists(destinationPath))
        {
          await DownloadFileAsync(client, fileUrl, destinationPath);
        }
        else
        {
          Console.WriteLine($"File already exists: {destinationPath}");
        }
      }

      Console.WriteLine("Download complete.");
    }
    catch (Exception ex)
    {
      Console.WriteLine($"An error occurred: {ex.Message}");
    }
  }

  private static async Task DownloadFileAsync(HttpClient client, string url, string destinationPath)
  {
    try
    {
      using HttpResponseMessage response = await client.GetAsync(url);
      response.EnsureSuccessStatusCode();

      using FileStream fs = new FileStream(destinationPath, FileMode.Create, FileAccess.Write, FileShare.None);
      await response.Content.CopyToAsync(fs);

      Console.WriteLine($"Downloaded: {destinationPath}");
    }
    catch (Exception ex)
    {
      Console.WriteLine($"Failed to download {url}: {ex.Message}");
    }
  }

  private static string[] ExtractFileUrls(string markdownContent)
  {
    var matches = Regex.Matches(markdownContent, "\\((https://.*?\\.(onnx|onnx\\.json))\\)");
    string[] fileUrls = new string[matches.Count];

    for (int i = 0; i < matches.Count; i++)
    {
      fileUrls[i] = matches[i].Groups[1].Value;
    }

    return fileUrls;
  }

  private static string GetLanguageCode(string fileName)
  {
    var match = Regex.Match(fileName, "^(\\w+_\\w+)-");
    return match.Success ? match.Groups[1].Value : string.Empty;
  }
}
