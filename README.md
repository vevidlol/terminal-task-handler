# C++ CLI Task Manager

A streamlined, object-oriented Command-Line Interface (CLI) Task Manager built in standard C++. This application enables full CRUD lifecycle tracking for tasks directly inside the terminal, storing live application data using structural text-file state persistence.

## Key Features
- **Full Data Persistence:** Automatically parses and structures active state changes out to a local `tasks.txt` file immediately following modifications.
- **Input Validation Safeties:** Utilizes clear standard terminal stream loops to isolate numeric selection parsing from breaking due to erratic character string submissions.
- **Clean Table Formatting:** Organizes dynamic record listings inside clear column width distributions using custom `<iomanip>` standard manipulators.

## Architectural Breakdown
The program utilizes strict Object-Oriented Design principles to detach active entities from structural controller flows:
- `Task Class`: Manages granular status properties, mutation signatures, and serialization formatting.
- `TaskManager Class`: Controls structural in-memory entities inside a dynamic vector container while managing parsing routines over standard File Streams (`<fstream>`).

## How to Run Locally

### Prerequisites
Ensure you have a standard C++ compiler ready (e.g., GCC/g++ or MSVC).

### Execution Steps
1. Clone the repository down into a working workspace:
   ```bash
   git clone [https://github.com/YOUR_USERNAME/YOUR_REPO_NAME.git](https://github.com/YOUR_USERNAME/YOUR_REPO_NAME.git)
