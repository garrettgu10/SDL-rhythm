#pragma once
#include <stdio.h>
#include <vector>
#include "note.h"
#include "lane.h"

struct BeatMapNote {
    double time;
    int laneNo;
} __attribute__((packed));

void writeBeatMap(const char *path, std::vector<std::vector<Note *>> lanes) {
    FILE *fp;
    
    long totalNotes = 0;
    for(auto lane : lanes){
        totalNotes += lane.size();
    }

    BeatMapNote *allNotes = new BeatMapNote[totalNotes];
    long currentNoteIndex = 0;
    
    for(int i = 0; i < lanes.size(); i++) {
        for(Note *note : lanes[i]) {
            allNotes[currentNoteIndex++] = {note->time, i};
        }
    }

    fp = fopen(path, "w");
    fwrite(allNotes, sizeof(BeatMapNote), totalNotes, fp);
    fclose(fp);
}

void readBeatMap(const char *path, std::vector<Lane *> &lanes) {
    FILE *fp;
    
    fp = fopen(path, "r");
    fseek(fp, 0, SEEK_END);
    long totalNotes = ftell(fp) / sizeof(BeatMapNote);
    rewind(fp);

    BeatMapNote *allNotes = new BeatMapNote[totalNotes];

    fread(allNotes, sizeof(BeatMapNote), totalNotes, fp);

    for(int i = 0 ; i < totalNotes; i++){
        BeatMapNote note = allNotes[i];
        lanes.at(note.laneNo)->futureNotes.push_back(new Note(note.time));
    }

    fclose(fp);
}