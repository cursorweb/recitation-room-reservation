#include "room_reservation.hpp"

#include <utility>

void RoomReservation::AddTeamMember(const std::string& student) {
  if (rooms_reserved_ == 0 ||
      rooms_reserved_ * room_capacity_ == athlete_count_) {
    auto** nr = new std::string*[rooms_reserved_ + 1];
    if (rooms_ != nullptr) {
      for (int i = 0; i < rooms_reserved_; i++) {
        // rooms_[i] is a string*
        nr[i] = rooms_[i];
      }
      delete[] rooms_;
    }

    nr[rooms_reserved_] = new std::string[room_capacity_];
    rooms_ = nr;
    rooms_reserved_++;
  }

  int oc = GetOccupantCount(rooms_reserved_ - 1);
  rooms_[rooms_reserved_ - 1][oc] = student;
  athlete_count_++;
}

void RoomReservation::RemoveTeamMember(const std::string& student) {
  for (int r = 0; r < rooms_reserved_; r++) {
    for (int c = 0; c < room_capacity_; c++) {
      if (rooms_[r][c] == student) {
        rooms_[r][c].clear();

        // first swap the row
        for (int i = c + 1; i < room_capacity_; i++) {
          std::swap(rooms_[r][i], rooms_[r][i - 1]);
        }

        // then swap the rest
        for (int i = r + 1; i < rooms_reserved_; i++) {
          // first take the last guy
          std::swap(rooms_[i - 1][room_capacity_ - 1], rooms_[i][0]);
          // then do the swap again
          for (int j = 1; j < room_capacity_; j++) {
            std::swap(rooms_[i][j], rooms_[i][j - 1]);
          }
        }

        bool last_empty = true;
        for (int i = 0; i < rooms_reserved_; i++) {
          if (!rooms_[rooms_reserved_ - 1][i].empty()) {
            last_empty = false;
            break;
          }
        }

        if (last_empty) {
          delete[] rooms_[rooms_reserved_ - 1];
          rooms_reserved_--;
        }

        if (rooms_reserved_ == 0) {
          delete[] rooms_;
          rooms_ = nullptr;
        }

        athlete_count_--;

        return;
      }
    }
  }

  throw std::runtime_error("Student not found");
}
void RoomReservation::MoveHotels(int new_room_capacity) {
  // reservation count
  int resc = (int)std::ceil((double)athlete_count_ / new_room_capacity);

  auto** nr = new std::string*[resc];

  int index = 0;

  for (int i = 0; i < resc; i++) {
    nr[i] = new std::string[new_room_capacity];
    for (int j = 0; j < new_room_capacity; j++) {
      int row = index / room_capacity_;
      int col = index % room_capacity_;
      nr[i][j] = rooms_[row][col];
      index++;
    }
  }

  // dealloc
  for (int i = 0; i < rooms_reserved_; i++) {
    delete[] rooms_[i];
  }
  delete[] rooms_;

  rooms_reserved_ = resc;
  rooms_ = nr;
  room_capacity_ = new_room_capacity;
}

// DO NOT MODIFY
RoomReservation::RoomReservation(int room_capacity):
    room_capacity_(room_capacity) {}
RoomReservation::RoomReservation(const std::string& filename,
                                 int room_capacity):
    room_capacity_(room_capacity) {
  std::ifstream ifs(filename);
  std::string student;
  while (ifs >> std::quoted(student)) {
    std::cout << student << std::endl;
    AddTeamMember(student);
  }
}
RoomReservation::~RoomReservation() {
  Clear();
}

int RoomReservation::RoomsReserved() const {
  return rooms_reserved_;
}
int RoomReservation::RoomCapacity() const {
  return room_capacity_;
}

void RoomReservation::Clear() {
  if (rooms_ == nullptr) return;
  for (int i = 0; i < rooms_reserved_; i++) {
    delete[] rooms_[i];
  }
  delete[] rooms_;
  rooms_ = nullptr;
  rooms_reserved_ = 0;
  athlete_count_ = 0;
}


std::string* RoomReservation::GetRoom(int room_number) {
  if (room_number < 0 || room_number >= rooms_reserved_ || rooms_ == nullptr) {
    return nullptr;
  }
  return rooms_[room_number];
}
int RoomReservation::GetOccupantCount(int room_number) {
  std::string* room = GetRoom(room_number);
  if (room == nullptr) {
    return 0;
  }
  int occupants = 0;
  for (int i = 0; i < room_capacity_; ++i) {
    if (!room[i].empty()) occupants++;
  }
  return occupants;
}

void RoomReservation::Print() {
  for (int i = 0; i < rooms_reserved_; ++i) {
    std::cout << "----------------\n";
    std::cout << "Room " << i << std::endl;
    for (int j = 0; j < room_capacity_; ++j) {
      std::string name = (rooms_[i][j] == "") ? "xxxxxxxxx" : rooms_[i][j];
      std::cout << name << '\n';
    }
    std::cout << "----------------\n";
  }
}