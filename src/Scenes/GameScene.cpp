#include "GameScene.hpp"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include "../ImGui/ImSequencer.h"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_internal.h"
#include "../Utils/Vec.hpp"

SDL_Texture* m_bg_tx_game = nullptr;

std::vector<std::string> m_files;

static const char* SequencerItemTypeNames[20];
int m_current_file_idx = 0;

struct MySequence : public ImSequencer::SequenceInterface
{
   // interface with sequencer

   virtual int GetFrameMin() const {
      return mFrameMin;
   }
   virtual int GetFrameMax() const {
      return mFrameMax;
   }
   virtual int GetItemCount() const { return (int)myItems.size(); }

   virtual int GetItemTypeCount() const { return sizeof(SequencerItemTypeNames) / sizeof(char*); }
   virtual const char* GetItemTypeName(int typeIndex) const { return SequencerItemTypeNames[typeIndex]; }
   virtual const char* GetItemLabel(int index) const
   {
      static char tmps[512];
	  const char* itemName = SequencerItemTypeNames[myItems[index].mType];
      if (itemName) {
			size_t len = strlen(itemName);
			if (len > 10) {
				snprintf(tmps, 512, "[%02d] %.10s..", index, itemName);
			} else {
				snprintf(tmps, 512, "[%02d] %s", index, itemName);
			}
		} else {
			snprintf(tmps, 512, "[%02d] %s", index, "Unknown");
		}
      return tmps;
   }

   virtual void Get(int index, int** start, int** end, int* type, unsigned int* color)
   {
      MySequenceItem& item = myItems[index];
      if (color)
         *color = 0xFFAA8080; // same color for everyone, return color based on type
      if (start)
         *start = &item.mFrameStart;
      if (end)
         *end = &item.mFrameEnd;
      if (type)
         *type = item.mType;
   }
   virtual void Add(int type) { myItems.push_back(MySequenceItem{ type, 0, 2, false }); };
   virtual void Del(int index) { 
	  myItems.erase(myItems.begin() + index);
	  m_current_file_idx--;
	}
   virtual void Duplicate(int index) { myItems.push_back(myItems[index]); }

   virtual size_t GetCustomHeight(int index) { return myItems[index].mExpanded ? 300 : 0; }

   // my datas
   MySequence() : mFrameMin(0), mFrameMax(0) {}
   int mFrameMin, mFrameMax;
   struct MySequenceItem
   {
      int mType;
      int mFrameStart, mFrameEnd;
      bool mExpanded;
   };
   std::vector<MySequenceItem> myItems;
   //RampEdit rampEdit;

   virtual void DoubleClick(int index) {
      if (myItems[index].mExpanded)
      {
         myItems[index].mExpanded = false;
         return;
      }
      for (auto& item : myItems)
         item.mExpanded = false;
      myItems[index].mExpanded = !myItems[index].mExpanded;
   }

   virtual void CustomDraw(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& legendRect, const ImRect& clippingRect, const ImRect& legendClippingRect)
   {
      static const char* labels[] = { "Translation", "Rotation" , "Scale" };

      //rampEdit.mMax = ImVec2(float(mFrameMax), 1.f);
      //rampEdit.mMin = ImVec2(float(mFrameMin), 0.f);
      draw_list->PushClipRect(legendClippingRect.Min, legendClippingRect.Max, true);
      for (int i = 0; i < 3; i++)
      {
         ImVec2 pta(legendRect.Min.x + 30, legendRect.Min.y + i * 14.f);
         ImVec2 ptb(legendRect.Max.x, legendRect.Min.y + (i + 1) * 14.f);
         //draw_list->AddText(pta, rampEdit.mbVisible[i] ? 0xFFFFFFFF : 0x80FFFFFF, labels[i]);
         //if (ImRect(pta, ptb).Contains(ImGui::GetMousePos()) && ImGui::IsMouseClicked(0))
            //rampEdit.mbVisible[i] = !rampEdit.mbVisible[i];
      }
      draw_list->PopClipRect();

      ImGui::SetCursorScreenPos(rc.Min);
      //ImCurveEdit::Edit(rampEdit, rc.Max - rc.Min, 137 + index, &clippingRect);
   }

   virtual void CustomDrawCompact(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& clippingRect)
   {
      //rampEdit.mMax = ImVec2(float(mFrameMax), 1.f);
      //rampEdit.mMin = ImVec2(float(mFrameMin), 0.f);
      draw_list->PushClipRect(clippingRect.Min, clippingRect.Max, true);
      for (int i = 0; i < 3; i++)
      {
         /*for (int j = 0; j < rampEdit.mPointCount[i]; j++)
         {
            float p = rampEdit.mPts[i][j].x;
            if (p < myItems[index].mFrameStart || p > myItems[index].mFrameEnd)
               continue;
            float r = (p - mFrameMin) / float(mFrameMax - mFrameMin);
            float x = ImLerp(rc.Min.x, rc.Max.x, r);
            draw_list->AddLine(ImVec2(x, rc.Min.y + 6), ImVec2(x, rc.Max.y - 4), 0xAA000000, 4.f);
         }*/
      }
      draw_list->PopClipRect();
   }
};

struct VideoFrame{
	std::string m_file_path;
	SDL_Texture* m_texture;
	int* frame_start;
	int* frame_end;
};

MySequence mySequence;
std::vector<VideoFrame> m_video_frames;

int m_current_frame = 0;
float timer = 0.0f;
float m_frame_time = 30.0f;
int m_max_frames = 10;

GameScene::GameScene(App *app, Logger *logger, Cooldown *cooldown, Camera *camera):Scene(app, logger, cooldown, camera)
{
	
}

void GameScene::load_assets()
{
    m_bg_tx_game = m_resources->GetAsset("main_scene")->GetTexture();
    
}

void GameScene::init()
{
    m_cd->set_state("init_event", .1f, [&] {	m_logger->log("Starting the GameScene!");});

    //
    load_assets();

	//
	for(int i = 0; i < sizeof(SequencerItemTypeNames) / sizeof(char*); i++){
		SequencerItemTypeNames[i] = "None";
	}
	//

   	mySequence.mFrameMin = 0;
   	mySequence.mFrameMax = 10;
}


void GameScene::update(double deltaTime)
{
	if(m_files.size() <= 0)return;

	for(int i = 0; i < m_video_frames.size(); i++){
		mySequence.Get(i, &m_video_frames[0].frame_start, &m_video_frames[0].frame_end, nullptr, nullptr);
	}

	timer += deltaTime;
	if(timer >= m_frame_time){
		timer = 0.0f;
		m_current_frame++;
		if(m_current_frame >= m_max_frames){
			m_current_frame = 0;
		}
	}
}

void GameScene::ui()
{
	{
		// let's create the sequencer
        static int selectedEntry = -1;
        static int firstFrame = 0;
        static bool expanded = true;
        static int currentFrame = 0;

        //Sequencer(&mySequence, &currentFrame, &expanded, &selectedEntry, &firstFrame, ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_ADD | ImSequencer::SEQUENCER_DEL | ImSequencer::SEQUENCER_COPYPASTE | ImSequencer::SEQUENCER_CHANGE_FRAME);
		ImGui::SetNextWindowPos(ImVec2(0, 210));
		ImGui::SetNextWindowSize(ImVec2(804, 170));
		if (ImGui::Begin("Timeline", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)) {
			ImGui::PushItemWidth(130);
			ImGui::InputInt("Frame Min", &mySequence.mFrameMin);
			ImGui::SameLine();
			ImGui::InputInt("Frame ", &currentFrame);
			ImGui::SameLine();
			ImGui::InputInt("Frame Max", &mySequence.mFrameMax);
			ImGui::PopItemWidth();
			// Inside the window, you can place your UI elements or other content
			// For example:
			Sequencer(&mySequence, &currentFrame, &expanded, &selectedEntry, &firstFrame, ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_ADD | ImSequencer::SEQUENCER_DEL | ImSequencer::SEQUENCER_COPYPASTE | ImSequencer::SEQUENCER_CHANGE_FRAME);
			
			// End the window
			ImGui::End();
		}
	}
}

//the main method for the image to gif feature
void convert(){
    std::string batchFilePath = "convert_images.bat";

    std::ofstream batchFile(batchFilePath);
    if (!batchFile.is_open()) {
        std::cerr << "Failed to create/open batch file." << std::endl;
    }

	//first we try to create the files txt
	{
		std::ofstream outfile("files.txt"); // Open file for writing

		// Check if the file is successfully opened
		if (!outfile.is_open()) {
			std::cerr << "Error opening file for writing!" << std::endl;
			return;
		}

		for(int i = 0; i < mySequence.myItems.size(); i++){
			int *x;
			int *y;
			int sum;

			mySequence.Get(i, &x, &y, nullptr, nullptr);
			sum = *y-*x;

			std::cout << *x << " " << *y << " " << sum << std::endl;

			outfile << "file image" << i+1 << ".jpeg" << std::endl;
    		outfile << "outpoint " << sum << std::endl;
		}

		// Close the file
		outfile.close();
	}

    batchFile << "@echo off" << std::endl;
    for (size_t i = 0; i < m_files.size(); ++i) {
        batchFile << "copy \"" << m_files[i] << "\" ." << std::endl;
        batchFile << "ren \"" << m_files[i].substr(m_files[i].find_last_of("\\") + 1) << "\" \"image" << i + 1 << ".jpeg\"" << std::endl;
    }
    //batchFile << "ffmpeg -i image%%d.jpeg -vf \"pad=180:100:(ow-iw)/2:(oh-ih)/2\" -t 5 output.gif" << std::endl;
    batchFile << "ffmpeg -f concat -i files.txt -vf \"pad=1600:1200:(ow-iw)/2:(oh-ih)/2\" -t 15 output.mp4" << std::endl;
    for (size_t i = 0; i < m_files.size(); ++i) {
        batchFile << "del image" << i + 1 << ".jpeg" << std::endl;
    }

    batchFile.close();

    //std::remove("convert_images.bat");

    int result = system(batchFilePath.c_str());
    if (result != 0) {
        std::cerr << "Command execution failed." << std::endl;
    }
}

void GameScene::draw()
{
	//ui
	GUI::draw([this](){this->ui();});

	m_atlas->draw(20, 20, std::to_string(m_current_frame).c_str(), m_app->get_main_font(), {255,255,255});
}

void GameScene::input(SDL_Event event)
{
	//partial scenes
	for(auto& scene : m_partial_scenes){
		scene->input(event);
	}

	switch (event.type) {
		case SDL_DROPFILE:
            m_files.push_back(event.drop.file);

			SequencerItemTypeNames[m_current_file_idx] = event.drop.file;
			mySequence.myItems.push_back(MySequence::MySequenceItem{ m_current_file_idx, 0, 2, false });

			m_video_frames.push_back(VideoFrame{event.drop.file, m_resources->LoadTexture(event.drop.file)});

			m_current_file_idx++;

			std::cout << "File dropped on window: " << event.drop.file << std::endl;
			//convert_file();
		break;

		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button) {
				case SDL_BUTTON_LEFT:

				break;
			}

		case SDL_MOUSEBUTTONUP:
			if(event.type == SDL_MOUSEBUTTONDOWN)return;
			switch (event.button.button) {
				case SDL_BUTTON_LEFT:			

				break;
			}

		case SDL_KEYDOWN:
			switch (event.key.keysym.scancode) {
				case SDL_SCANCODE_D:

				break;
				case SDL_SCANCODE_F:
					convert();
					break;
				case SDL_SCANCODE_E:
					m_files.clear();

					/*if(m_file_path != ""){
						convert_file(m_file_path);
					}*/
				break;
				case SDL_SCANCODE_A:
					
				break;
				case SDL_SCANCODE_T:
					
				break;
				case SDL_SCANCODE_RETURN:
					
				break;
				case SDL_SCANCODE_ESCAPE:
					
				break;
				case SDL_SCANCODE_TAB:
					
					
				break;
			}
		break;
		case SDL_KEYUP:
			switch (event.key.keysym.scancode) {
				case SDL_SCANCODE_D:
				break;
				case SDL_SCANCODE_RETURN:
					
				break;
				case SDL_SCANCODE_TAB:
					
				break;
			}
		break;
	}
}

void GameScene::clean()
{

}

