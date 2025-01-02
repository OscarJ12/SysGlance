# SysGlance

SysGlance is a lightweight terminal-based system monitoring tool for Linux. It provides essential system metrics in a clean and efficient way, perfect for minimalists or those who prefer the command line.

## Features

- **CPU Usage**: Real-time usage with a visual text-based graph.
- **Memory Usage**: Displays total, used, and free memory.
- **Disk Usage**: Lists device sizes, usage, and available space in a clean table.
- **Network Activity**: Monitors received and transmitted bytes per interface.
- **User Information**: Displays the currently logged-in user.
- **System Information**: Shows hostname, OS version, uptime, and current time.

## Installation

### From the `.deb` File

1. Download the `.deb` package from the [releases page](https://github.com/OscarJ12/SysGlance/releases).
2. Install the package:
   ```bash
   sudo dpkg -i SysGlance-1.0.deb
   ```
3. Run the program:
   ```bash
   sysglance
   ```

### From the `.tar.gz` File

1. Download the `SysGlance.tar.gz` file from the [releases page](https://github.com/OscarJ12/SysGlance/releases).
2. Extract the archive:
   ```bash
   tar -xvzf SysGlance.tar.gz
   cd SysGlance
   ```
3. Build and run:
   ```bash
   make
   ./sysglance
   ```

## Usage

Simply run the program in your terminal:
```bash
sysglance
```

The system metrics will refresh every 2 seconds. Press `Ctrl+C` to exit.

## Requirements

- Linux-based operating system
- GCC compiler (for building from source)

## Screenshots

![SysGlance in Action](https://example.com/screenshot.png) *(Add a real screenshot URL)*

## Contributing

Contributions are welcome! Feel free to fork the repository and submit a pull request.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contact

For questions or suggestions, please contact:
- GitHub: [OscarJ12](https://github.com/OscarJ12)
- Email: [your.email@example.com](mailto:your.email@example.com)
