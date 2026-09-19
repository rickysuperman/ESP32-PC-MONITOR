import requests
import serial
import time


esp = serial.Serial("COM3", 115200, timeout=1)
time.sleep(2)

while True:
    data = requests.get("http://localhost:8085/data.json").json()

    hardware = data["Children"][0]["Children"]

    # =====================
    # CPU
    # =====================
    cpu = next(
        x for x in hardware
        if "i5-13500" in x["Text"]
    )

    cpu_temp_group = next(
        x for x in cpu["Children"]
        if x["Text"] == "Temperatures"
    )

    cpu_temp = next(
        x for x in cpu_temp_group["Children"]
        if x["Text"] == "CPU Package"
    )

    cpu_load_group = next(
        x for x in cpu["Children"]
        if x["Text"] == "Load"
    )

    cpu_load = next(
        x for x in cpu_load_group["Children"]
        if x["Text"] == "CPU Total"
    )

    # =====================
    # GPU
    # =====================
    gpu = next(
        x for x in hardware
        if "RTX 4060 Ti" in x["Text"]
    )

    gpu_temp_group = next(
        x for x in gpu["Children"]
        if x["Text"] == "Temperatures"
    )

    gpu_temp = next(
        x for x in gpu_temp_group["Children"]
        if x["Text"] == "GPU Core"
    )

    gpu_load_group = next(
        x for x in gpu["Children"]
        if x["Text"] == "Load"
    )

    gpu_load = next(
        x for x in gpu_load_group["Children"]
        if x["Text"] == "GPU Core"
    )

    # =====================
    # 顯示在電腦
    # =====================
    print(
        "CPU:", cpu_temp["Value"], cpu_load["Value"],
        "| GPU:", gpu_temp["Value"], gpu_load["Value"]
    )

    # =====================
    # 傳給 ESP32
    # =====================
    message = (
        "CPU_TEMP:" + cpu_temp["Value"] +
        ",CPU_LOAD:" + cpu_load["Value"] +
        ",GPU_TEMP:" + gpu_temp["Value"] +
        ",GPU_LOAD:" + gpu_load["Value"] +
        "\n"
    )

    esp.write(message.encode())
    print(esp.readline().decode(errors="ignore").strip())

    time.sleep(2)
