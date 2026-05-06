import time
import random
import os

# ── FILER (samme som app.py og C++ main) ──────────────────────────────────────
CMD_FILE     = "command.txt"
LIVE_FILE    = "live_data.txt"
SESSION_FILE = "session_data.txt"

# ── KONSTANTER (samme som MaaleSession og Calculator) ─────────────────────────
WATER_PRICE   = 0.065    # kr/L  (danva statistik)
ENERGY_PRICE  = 2.5      # kr/kWh (danmarks statistik 2025)
SPECIFIC_HEAT = 4186     # J/(kg*C)
JOULE_TO_KWH  = 3600000
COLD_WATER    = 8        # graders koldt vand ind (bruges i DeltaTemp)

# ── CALCULATOR (spejler Calculator.cpp) ───────────────────────────────────────
def calculate_average_temp(temp_sum, reading_count):
    if reading_count == 0:
        return 0.0
    return temp_sum / reading_count

def calculate_energy_used(total_volume, avg_temp):
    # samme formel som Calculator::calculateEnergyUsed med DeltaTemp
    delta_temp = avg_temp - COLD_WATER
    return (total_volume * SPECIFIC_HEAT * delta_temp) / JOULE_TO_KWH

def calculate_total_water_cost(total_volume, water_price):
    return total_volume * water_price

def calculate_total_energy_cost(energy_used, energy_price):
    return energy_used * energy_price

# ── AFSPILLYD (spejler AfspilLyd.cpp checkAndPlay) ────────────────────────────
def check_and_play(total_volume, next_threshold, water_limit):
    if total_volume >= next_threshold:
        print(f"[LYD] Afspiller lyd – {round(total_volume, 1)}L nået (grænse: {next_threshold}L)")
        while total_volume >= next_threshold:
            next_threshold += water_limit
    return next_threshold

# ── HOVEDLOOP (spejler main.cpp) ──────────────────────────────────────────────
def simulate():
    print("=" * 50)
    print("Smart Shower SIMULATOR")
    print("Kører igennem samme logik som C++ programmet")
    print("Start Flask i en anden terminal: python app.py")
    print("=" * 50)

    session_active  = False
    total_volume    = 0.0
    temp_sum        = 0.0
    reading_count   = 0
    current_user    = 0
    water_limit     = 10.0
    next_threshold  = 10.0
    start_time      = None

    while True:
        # ── Læs kommando fra Flask (samme som C++ main-loop) ──────────────────
        command = ""
        if os.path.exists(CMD_FILE):
            with open(CMD_FILE, "r") as f:
                command = f.readline().strip()
            if command:
                print(f"\n[CMD] Kommando modtaget: {command}")
                open(CMD_FILE, "w").close()  # nulstil filen så den ikke læses igen

        # ── START:1:15 → MeasurementControl::startSession() ──────────────────
        if command.startswith("START:"):
            parts        = command.split(":")
            current_user = int(parts[1])
            water_limit  = float(parts[2])
            # nulstil alle værdier som startSession() gør
            total_volume   = 0.0
            temp_sum       = 0.0
            reading_count  = 0
            next_threshold = water_limit   # AfspilLyd::reset()
            start_time     = time.time()
            session_active = True
            print(f"[START] Bruger {current_user} – lydinterval {water_limit}L")

        # ── STOP → MeasurementControl::stopSession() ──────────────────────────
        if command == "STOP" and session_active:
            shower_length     = int(time.time() - start_time)
            avg_temp          = calculate_average_temp(temp_sum, reading_count)
            energy_used       = calculate_energy_used(total_volume, avg_temp)
            total_water_cost  = calculate_total_water_cost(total_volume, WATER_PRICE)
            total_energy_cost = calculate_total_energy_cost(energy_used, ENERGY_PRICE)
            total_cost        = total_water_cost + total_energy_cost

            # MeasurementControl::storeSession() – samme format som C++
            with open(SESSION_FILE, "a") as f:
                f.write("----- Session -----\n")
                f.write(f"UserId: {current_user}\n")
                f.write(f"ShowerLength: {shower_length} sekunder\n")
                f.write(f"TotalWaterUsed: {round(total_volume, 2)} liter\n")
                f.write(f"AverageTemp: {round(avg_temp, 2)} C\n")
                f.write(f"EnergyUsed: {round(energy_used, 4)}\n")
                f.write(f"WaterPrice: {WATER_PRICE} kr/L\n")
                f.write(f"EnergyPrice: {ENERGY_PRICE} kr\n")
                f.write(f"TotalWaterCost: {round(total_water_cost, 2)} kr\n")
                f.write(f"TotalEnergyCost: {round(total_energy_cost, 2)} kr\n")
                f.write(f"TotalCost: {round(total_cost, 2)} kr\n")
                f.write(f"WaterLimit: {water_limit} liter\n")
                f.write("-------------------\n")

            print(f"[STOP] Session afsluttet:")
            print(f"       Vand:       {round(total_volume, 1)} L")
            print(f"       Temp:       {round(avg_temp, 1)} C")
            print(f"       Energi:     {round(energy_used, 4)} kWh")
            print(f"       Vandpris:   {round(total_water_cost, 2)} kr")
            print(f"       Energipris: {round(total_energy_cost, 2)} kr")
            print(f"       Total:      {round(total_cost, 2)} kr")
            print(f"       Varighed:   {shower_length} sek")
            session_active = False

        # ── Måleloop → MeasurementControl::beginMeasurement() ────────────────
        if session_active:
            # simulerede sensorværdier (FlowSensor + TemperatureSensor)
            current_temp = round(random.uniform(36.0, 42.0), 1)   # grader
            current_flow = round(random.uniform(0.08, 0.18), 4)   # liter pr sekund

            total_volume  += current_flow
            temp_sum      += current_temp
            reading_count += 1

            avg_temp    = calculate_average_temp(temp_sum, reading_count)
            energy_used = calculate_energy_used(total_volume, avg_temp)
            total_cost  = calculate_total_water_cost(total_volume, WATER_PRICE) \
                        + calculate_total_energy_cost(energy_used, ENERGY_PRICE)

            # live_data.txt – samme format som C++ beginMeasurement()
            with open(LIVE_FILE, "w") as f:
                f.write(f"water={round(total_volume, 2)}\n")
                f.write(f"temp={current_temp}\n")
                f.write(f"cost={round(total_cost, 2)}\n")

            print(f"[MAALING] {round(total_volume,1)}L | {current_temp}C | {round(total_cost,2)}kr", end="\r")

            # AfspilLyd::checkAndPlay()
            next_threshold = check_and_play(total_volume, next_threshold, water_limit)

        time.sleep(1)  # samme som C++ sleep_for(1 sekund)

if __name__ == "__main__":
    simulate()
