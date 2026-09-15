from urllib.request import urlopen

# Change this to your actual ESP32 IP
ESP32_IP = "YOUR_ESP32_IP"

while True:

    command = input(
        "Enter plastic, glass, metal, general or q: "
    ).strip().lower()

    if command == "q":
        print("Exiting...")
        break

    if command not in [
        "plastic",
        "glass",
        "metal",
        "general"
    ]:
        print("Invalid command")
        continue

    url = f"http://{ESP32_IP}/{command}"

    try:

        response = urlopen(
            url,
            timeout=3
        )

        message = response.read().decode()

        print("ESP32:", message)

    except Exception as e:

        print(
            "Communication error:",
            e
        )
