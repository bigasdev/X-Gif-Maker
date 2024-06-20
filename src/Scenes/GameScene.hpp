#ifndef _GAMESCENE_HPP
#define _GAMESCENE_HPP
#include "Scene.hpp"
#include "../ImGui/ImSequencer.h"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_internal.h"

#include <ctime> 
#include <functional>

static const char* SequencerItemTypeNames[20] = {
   "None",
   "None",
   "None",
   "None",
   "None",
   "None",
   "None",
   "None",
   "None",
   "None",
   "None",
   "None",
   "None",
   "None",
   "None",
   "None",
   "None",
   "None",
   "None",
   "None"
};

struct Timeline : public ImSequencer::SequenceInterface
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
				snprintf(tmps, 512, "[%02d] %.20s..", index, itemName);
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
     if(m_del_callback) m_del_callback(index);
	  myItems.erase(myItems.begin() + index);
	}
   virtual void Duplicate(int index) { 
      if(m_duplicated_callback) m_duplicated_callback(index);
      myItems.push_back(myItems[index]); 
   }

   virtual size_t GetCustomHeight(int index) { return myItems[index].mExpanded ? 300 : 0; }

   // my datas
   Timeline() : mFrameMin(0), mFrameMax(0) {}
   int mFrameMin, mFrameMax;
   struct MySequenceItem
   {
      int mType;
      int mFrameStart, mFrameEnd;
      bool mExpanded;
   };
   std::vector<MySequenceItem> myItems;
   std::function<void(int)> m_del_callback;
   std::function<void(int)> m_duplicated_callback;
   //RampEdit rampEdit;

   virtual void DoubleClick(int index) {
      /*if (myItems[index].mExpanded)
      {
         myItems[index].mExpanded = false;
         return;
      }
      for (auto& item : myItems)
         item.mExpanded = false;
      myItems[index].mExpanded = !myItems[index].mExpanded;*/
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

struct GifFrame{
	std::string m_file_path;
	std::string m_file_name;
	SDL_Texture* m_texture;
	int frame_start;
	int frame_end;
};
struct GifSettings{
	bool m_is_transparent = false;
	bool m_is_looping = true;
	bool m_is_open_folder = true;
	int m_width = 0;
	int m_height = 0;
   int fps = 60;
};

class GameScene : public Scene {
public:
	GameScene(App* app, Logger* logger, Cooldown* cooldown, Camera* camera);

	void init();
	void update(double deltaTime);
	void ui();
	void draw();
	void input(SDL_Event event);
	void clean();
	void load_assets();


private:
	
};

#endif
