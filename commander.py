import serial
import uvicorn
from fastapi import FastAPI
from pydantic import BaseModel
import cv2
from fastapi.responses import StreamingResponse, HTMLResponse
from core.autonomous_driving import autonomous_drive
from core.helper import parse_input_command_list


app = FastAPI()
ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
ser.flush()


class MoveCommand(BaseModel):
    pressedKeysSet: list
    speed: int


def generate_frame_from_video_stream_connection():

    cap = cv2.VideoCapture(0)
    while True:

        ret, frame = cap.read()
        autonomous_drive(frame)
        ret, jpeg = cv2.imencode('.jpg', frame)

        frame = jpeg.tobytes()
        if not ret:
            return

        yield (b'--frame\r\n' b'Content-Type: image/jpeg\r\ncontent-length: ' +
               str(len(frame)).encode() + b'\r\n\r\n' + frame + b'\r\n\r\n')


@app.get('/video')
def video_feed():
    return StreamingResponse(generate_frame_from_video_stream_connection(),
                             media_type="multipart/x-mixed-replace; boundary=frame")


@app.get('/', response_class=HTMLResponse)
def main():
    with open("./index.html", "r") as fl:
        return fl.read()


@app.post("/move")
def move_car(input_data: MoveCommand):
    pressedKeysSet = input_data.pressedKeysSet
    direction = pressedKeysSet
    if len(direction) == 0:
        direction = "STOP"
    else:
        direction = parse_input_command_list(direction)

    speed = input_data.speed
    command = f"{direction}#{speed}"
    print(command)
    ser.reset_output_buffer()
    ser.write(f"{command}\n".encode('utf-8'))
    line = ser.readline().decode('utf-8').rstrip()
    print(line)
    return {"response": command}


if __name__ == '__main__':
    uvicorn.run("commander:app", host="0.0.0.0", port=5000, interface="asgi3")
