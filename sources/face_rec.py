import face_recognition
import os 

# Load the jpg files into numpy arrays
def recognize_face(path):
    registered_list = []
    known_faces = []
    unknown_image = face_recognition.load_image_file("./known/1.jpg")
    files = [f for f in os.listdir('./known') if (os.path.isfile('./known/' + f) and f.split('.')[1] == 'jpg')]

    for f in files:
        registered_list.append(face_recognition.load_image_file('./known/' + f))
# Get the face encodings for each face in each image file
# Since there could be more than one face in each image, it returns a list of encodings.
# But since I know each image only has one face, I only care about the first encoding in each image, so I grab index 0.
    try:
        for r in registered_list:
            known_faces.append(face_recognition.face_encodings(r)[0])
    except IndexError:
        return False  

    print(path)
    unknown_face_encoding = face_recognition.face_encodings(unknown_image)[0]
# results is an array of True/False telling if the unknown face matched anyone in the known_faces array
    results = face_recognition.compare_faces(known_faces, unknown_face_encoding)
    i = 0
    for result in results:
        if result == True:
            return i
        i += 1

    return -1 
