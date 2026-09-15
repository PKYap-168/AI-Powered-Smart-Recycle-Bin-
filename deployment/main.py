import cv2
import time
from urllib.request import urlopen
from ultralytics import YOLO

# =========================================================
# SETTINGS
# =========================================================

MODEL_PATH = "best.pt"

# Change this to your actual ESP32 IP address
ESP32_IP = "10.207.154.172" # Example local IP; change to your ESP32 address

# Minimum YOLO confidence required
CONFIDENCE_THRESHOLD = 0.75

# Same class must be detected continuously for this long
DETECTION_TIME_THRESHOLD = 1.5

# Prevent repeated triggering too quickly
COOLDOWN_SECONDS = 5.0


# =========================================================
# LOAD YOLO MODEL
# =========================================================

model = YOLO(MODEL_PATH)

print("Model classes:")
print(model.names)


# =========================================================
# OPEN CAMERA
# =========================================================

camera = cv2.VideoCapture(0)

if not camera.isOpened():
    print("ERROR: Camera cannot be opened")
    exit()


# =========================================================
# DETECTION STATE VARIABLES
# =========================================================

current_candidate = None
candidate_start_time = None

last_trigger_time = 0


# =========================================================
# SEND COMMAND TO ESP32
# =========================================================

def send_command(command):

    url = f"http://{ESP32_IP}/{command}"

    try:

        response = urlopen(
            url,
            timeout=3
        )

        message = response.read().decode()

        print(f"ESP32: {message}")

        return True

    except Exception as e:

        print(
            f"ESP32 communication error: {e}"
        )

        return False


# =========================================================
# MAIN LOOP
# =========================================================

while True:

    ret, frame = camera.read()

    if not ret:
        print("ERROR: Failed to read camera frame")
        break


    # =====================================================
    # YOLO DETECTION
    # =====================================================

    results = model.predict(
        source=frame,
        imgsz=640,
        conf=CONFIDENCE_THRESHOLD,
        device=0,
        verbose=False
    )

    result = results[0]


    detected_class = None
    highest_confidence = 0.0


    # =====================================================
    # GET HIGHEST CONFIDENCE DETECTION
    # =====================================================

    for box in result.boxes:

        confidence = float(box.conf[0])

        class_id = int(box.cls[0])

        class_name = model.names[class_id].lower()


        if confidence > highest_confidence:

            highest_confidence = confidence

            detected_class = class_name


    current_time = time.time()


    # =====================================================
    # CONTINUOUS DETECTION TIMER
    # =====================================================

    if detected_class is not None:

        # New class detected
        if detected_class != current_candidate:

            current_candidate = detected_class

            candidate_start_time = current_time


        # Same class still detected
        else:

            detected_duration = (
                current_time
                -
                candidate_start_time
            )


            # =================================================
            # TRIGGER AI BIN
            # =================================================

            if (
                detected_duration
                >=
                DETECTION_TIME_THRESHOLD
            ):

                if (
                    current_time
                    -
                    last_trigger_time
                    >=
                    COOLDOWN_SECONDS
                ):


                    # -----------------------------------------
                    # Plastic
                    # -----------------------------------------

                    if current_candidate == "plastic":

                        print(
                            f"PLASTIC CONFIRMED "
                            f"| Confidence: "
                            f"{highest_confidence:.2f} "
                            f"| Time: "
                            f"{detected_duration:.2f}s"
                        )

                        if send_command("plastic"):

                            last_trigger_time = (
                                current_time
                            )


                    # -----------------------------------------
                    # Glass
                    # -----------------------------------------

                    elif current_candidate == "glass":

                        print(
                            f"GLASS CONFIRMED "
                            f"| Confidence: "
                            f"{highest_confidence:.2f} "
                            f"| Time: "
                            f"{detected_duration:.2f}s"
                        )

                        if send_command("glass"):

                            last_trigger_time = (
                                current_time
                            )


                    # -----------------------------------------
                    # Metal
                    # -----------------------------------------

                    elif current_candidate == "metal":

                        print(
                            f"METAL CONFIRMED "
                            f"| Confidence: "
                            f"{highest_confidence:.2f} "
                            f"| Time: "
                            f"{detected_duration:.2f}s"
                        )

                        if send_command("metal"):

                            last_trigger_time = (
                                current_time
                            )


                    # Reset after triggering
                    current_candidate = None

                    candidate_start_time = None


    else:

        # Nothing detected
        # Reset detection timer

        current_candidate = None

        candidate_start_time = None


    # =====================================================
    # DISPLAY
    # =====================================================

    annotated_frame = result.plot()


    # -----------------------------------------------------
    # Show detection information
    # -----------------------------------------------------

    if detected_class is not None:

        if (
            current_candidate
            ==
            detected_class
            and
            candidate_start_time
            is not None
        ):

            elapsed = (
                current_time
                -
                candidate_start_time
            )

        else:

            elapsed = 0


        text = (
            f"{detected_class.upper()} "
            f"{highest_confidence:.2f} "
            f"| {elapsed:.1f}/"
            f"{DETECTION_TIME_THRESHOLD:.1f}s"
        )


        cv2.putText(
            annotated_frame,
            text,
            (20, 40),
            cv2.FONT_HERSHEY_SIMPLEX,
            0.8,
            (255, 255, 255),
            2
        )


    # -----------------------------------------------------
    # Keyboard instruction
    # -----------------------------------------------------

    cv2.putText(
        annotated_frame,
        "Q = Quit",
        (20, 75),
        cv2.FONT_HERSHEY_SIMPLEX,
        0.65,
        (255, 255, 255),
        2
    )


    cv2.imshow(
        "AI Recycling System",
        annotated_frame
    )


    # =====================================================
    # KEYBOARD CONTROL
    # =====================================================

    key = cv2.waitKey(1) & 0xFF


    # -----------------------------------------------------
    # G = General waste bin
    # -----------------------------------------------------

    if key == ord("g"):

        print("GENERAL WASTE manual command")

        send_command("general")


    # -----------------------------------------------------
    # Q = Quit
    # -----------------------------------------------------

    elif key == ord("q"):

        break


# =========================================================
# CLEANUP
# =========================================================

camera.release()

cv2.destroyAllWindows()

print("System stopped")
