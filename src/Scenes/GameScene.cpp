#include "GameScene.hpp"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include "../Utils/Vec.hpp"
#include "../Utils/Math.hpp"
#include "ConvertionHandler.hpp"
#include "../Utils/Gizmos.hpp"
#include "../Utils/Fini.hpp"
#include "../Core/DataLoader.hpp"

SDL_Texture *m_bg_texture = nullptr;
SDL_Texture *m_left_door = nullptr;
SDL_Texture *m_right_door = nullptr;

std::vector<std::string> m_files_path;

int m_current_file_idx = 0;

Timeline m_timeline;
std::vector<GifFrame> m_video_frames;

// control variables
std::string m_current_extension = "";
bool m_is_hovering = false;

// preview variables
int m_hover_frame = -99;
int m_current_frame = 0;
float timer = 0.0f;
float m_frame_time = 0.5f;
int m_max_frames = 10;
bool m_is_playing = false;

// door variables
float m_right_door_x = 0;
float m_left_door_x = 0;
float m_door_speed = 3.5f;
int m_right_door_pos = 340;
int m_left_door_pos = -330;

// data
IniData *m_preview_button;
IniData *m_convert_button;
std::unique_ptr<Fini> m_gifsettings_handler;

// private control variables
bool m_is_mouse_down = false;
bool m_is_hovering_preview = false;
bool m_is_hovering_convert = false;
bool m_edit_frame_popup = false;
bool m_frame_hover_tooltip = false;

// cursors
SDL_Cursor *cursor = nullptr;
SDL_Cursor *cursor_hand = nullptr;

// convertion variables
GifSettings m_gif_settings;

GameScene::GameScene(App *app, Logger *logger, Cooldown *cooldown, Camera *camera) : Scene(app, logger, cooldown, camera)
{
}

void GameScene::load_assets()
{
  m_bg_texture = m_resources->GetAsset("image_gif_scene")->GetTexture();
  m_left_door = m_resources->GetAsset("left_door")->GetTexture();
  m_right_door = m_resources->GetAsset("right_door")->GetTexture();

  cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
  cursor_hand = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
}

void GameScene::init()
{
  m_cd->set_state("init_event", .1f, [&]
                  { m_logger->log("Starting the GameScene!"); });
  m_app->change_background_color({25, 26, 50});

  //
  load_assets();

  m_gifsettings_handler = std::make_unique<Fini>("res/data/gif_img_settings.ini");

  m_gifsettings_handler->initialize_value("gif settings", "png", "false");
  m_gifsettings_handler->initialize_value("gif settings", "looping", "true");
  m_gifsettings_handler->initialize_value("gif settings", "open_folder", "true");

  m_gifsettings_handler->save();

  m_gifsettings_handler->grab_value("gif settings", "png", &m_gif_settings.m_is_transparent);
  m_gifsettings_handler->grab_value("gif settings", "looping", &m_gif_settings.m_is_looping);
  m_gifsettings_handler->grab_value("gif settings", "open_folder", &m_gif_settings.m_is_open_folder);

  // data stuff
  m_ini_handler->load_ini_files("config.ini");
  m_preview_button = m_ini_handler->get_ini_data("preview_img_button");
  m_convert_button = m_ini_handler->get_ini_data("convert_img_button");

  m_timeline.mFrameMin = 0;
  m_timeline.mFrameMax = m_gif_settings.fps * 10;
  m_timeline.m_del_callback = [&](int index)
  {
    m_video_frames.erase(m_video_frames.begin() + index);
    if (m_video_frames.size() <= 0)
      m_current_extension = "";
  };
  m_timeline.m_duplicated_callback = [&](int index)
  {
    m_video_frames.push_back(m_video_frames[index]);
  };
  m_timeline.on_frame_click = [&](int index)
  {
    m_current_frame = index;

    m_edit_frame_popup = true;
  };

  m_timeline.on_frame_hover = [&](int index)
  {
    if (m_hover_frame == index)
      return;

    m_hover_frame = index;

    F_Debug::log("Trying to set" + std::to_string(index));
    F_Debug::log("current frame" + std::to_string(m_hover_frame));

    m_frame_hover_tooltip = true;
  };

  m_timeline.on_frame_hover_exit = [&](int index)
  {
    if (m_hover_frame != index)
      return;

    m_hover_frame = -99;

    F_Debug::log("Hover exit");
    m_frame_hover_tooltip = false;
  };

  //
  m_right_door_x = 0;
  m_left_door_x = 0;
}

void GameScene::update(double deltaTime)
{
#if F_ENABLE_DEBUG
  m_ini_handler->update_ini_files();
#endif

  m_gifsettings_handler->set_value("gif settings", "png", m_gif_settings.m_is_transparent);
  m_gifsettings_handler->set_value("gif settings", "looping", m_gif_settings.m_is_looping);
  m_gifsettings_handler->set_value("gif settings", "open_folder", m_gif_settings.m_is_open_folder);

  if (Mouse::is_at_area({m_preview_button->relative_x, m_preview_button->relative_y, 25, 25}) || Mouse::is_at_area({m_convert_button->relative_x, m_convert_button->relative_y, 25, 25}))
  {
    m_is_hovering = true;
  }
  else
  {
    m_is_hovering = false;
  }

  m_left_door_x = Math::lerp(m_left_door_x, m_left_door_pos, m_door_speed * deltaTime);
  m_right_door_x = Math::lerp(m_right_door_x, m_right_door_pos, m_door_speed * deltaTime);

  if (m_files_path.size() <= 0)
    return;

  for (int i = 0; i < m_video_frames.size(); i++)
  {
    int *x;
    int *y;
    m_timeline.Get(i, &x, &y, nullptr, nullptr);

    m_video_frames[i].frame_start = *x;
    m_video_frames[i].frame_end = *y;
  }

  if (Mouse::is_at_area({m_preview_button->relative_x, m_preview_button->relative_y, 25, 25}))
  {
    m_is_hovering_preview = true;
    if (m_is_mouse_down)
    {
      m_is_playing = !m_is_playing;
      m_is_mouse_down = false;
    }
  }
  else
  {
    m_is_hovering_preview = false;
  }

  if (Mouse::is_at_area({m_convert_button->relative_x, m_convert_button->relative_y, 25, 25}))
  {
    m_is_hovering_convert = true;
    if (m_is_mouse_down)
    {
      auto path = Data_Loader::save_file("*.gif");
      std::cout << path << "\n";

      if (path != "")
      {
        Convertion::convert(m_video_frames, path, m_gif_settings, m_current_extension);
      }

      m_is_mouse_down = false;
    }
  }
  else
  {
    m_is_hovering_convert = false;
  }

  if (!m_is_playing)
    return;

  m_max_frames = m_gif_settings.fps * 10;

  timer += 15 * deltaTime;
  if (timer >= m_frame_time)
  {
    timer = 0.0f;
    m_current_frame++;
    if (m_current_frame >= m_max_frames)
    {
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

    if (m_is_playing)
    {
      currentFrame = m_current_frame;
    }
    else
    {
      m_current_frame = currentFrame;
    }

    if (m_is_hovering_preview)
    {
      ImGui::BeginTooltip();
      ImGui::Text("Preview of the gif");
      ImGui::EndTooltip();
    }

    if (m_is_hovering_convert)
    {
      ImGui::BeginTooltip();
      ImGui::Text("Convert the gif");
      ImGui::EndTooltip();
    }

    if (m_frame_hover_tooltip)
    {

      ImGui::BeginTooltip();
      std::string frame = "Frame " + std::to_string(m_hover_frame);
      ImGui::Text(frame.c_str());
      ImGui::Text("Frames: %d", m_timeline.myItems[m_hover_frame].mFrameEnd - m_timeline.myItems[m_hover_frame].mFrameStart);
      ImGui::Text("Duration in seconds : %f", static_cast<float>((m_timeline.myItems[m_hover_frame].mFrameEnd - m_timeline.myItems[m_hover_frame].mFrameStart)) / m_gif_settings.fps);
      ImGui::EndTooltip();
    }

    // Sequencer(&mySequence, &currentFrame, &expanded, &selectedEntry, &firstFrame, ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_ADD | ImSequencer::SEQUENCER_DEL | ImSequencer::SEQUENCER_COPYPASTE | ImSequencer::SEQUENCER_CHANGE_FRAME);
    ImGui::SetNextWindowPos(ImVec2(0, 210));
    ImGui::SetNextWindowSize(ImVec2(550, 170));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
    if (ImGui::Begin("Timeline", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
    {
      ImGui::PushItemWidth(130);
      ImGui::InputInt("Frame Min", &m_timeline.mFrameMin);
      ImGui::SameLine();
      ImGui::InputInt("Frame ", &currentFrame);
      ImGui::SameLine();
      ImGui::InputInt("Frame Max", &m_timeline.mFrameMax);
      ImGui::PopItemWidth();
      // Inside the window, you can place your UI elements or other content
      // For example:
      Sequencer(&m_timeline, &currentFrame, &expanded, &selectedEntry, &firstFrame, ImSequencer::SEQUENCER_DEL | ImSequencer::SEQUENCER_CHANGE_FRAME | ImSequencer::SEQUENCER_EDIT_STARTEND);

      // End the window
      ImGui::End();
    }
    ImGui::SetNextWindowPos(ImVec2(552, 210));
    ImGui::SetNextWindowSize(ImVec2(254, 170));
    if (ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
    {
      ImGui::Checkbox("Transparent background", &m_gif_settings.m_is_transparent);

      if (ImGui::IsItemHovered())
      {
        ImGui::BeginTooltip();
        ImGui::Text("WARNING: All the images need to be the same size to work with transparent background!");
        ImGui::EndTooltip();
      }

      ImGui::Checkbox("Loop", &m_gif_settings.m_is_looping);
      ImGui::Checkbox("Open folder after convertion", &m_gif_settings.m_is_open_folder);
      ImGui::End();
    }
    ImGui::PopStyleColor();

    if (m_edit_frame_popup)
    {
      ImGui::OpenPopup("Edit Frame");
    }

    if (ImGui::BeginPopupModal("Edit Frame", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
      ImGui::Text("Edit the frame");
      ImGui::InputInt("Frame Start", &m_timeline.myItems[selectedEntry].mFrameStart);
      ImGui::InputInt("Frame End", &m_timeline.myItems[selectedEntry].mFrameEnd);

      ImGui::Text("Duration in seconds : ");
      ImGui::SameLine();
      ImGui::Text(std::to_string(static_cast<float>((m_timeline.myItems[selectedEntry].mFrameEnd - m_timeline.myItems[selectedEntry].mFrameStart)) / m_gif_settings.fps).c_str());

      if (ImGui::Button("Close"))
      {
        m_edit_frame_popup = false;
        ImGui::CloseCurrentPopup();
      }

      ImGui::EndPopup();
    }
  }
}

void GameScene::draw()
{
  auto condition = [&](const GifFrame &vf)
  {
    return m_current_frame >= vf.frame_start && m_current_frame <= vf.frame_end;
  };

  auto frame = Vec::find_vec_element<GifFrame>(m_video_frames, condition);

  if (frame != nullptr)
  {
    m_atlas->draw(frame->m_texture, {256, 192}, 1, Math::mid(m_app->get_window_size().x, 256) - 18, 10, false);
  }

  m_atlas->draw(m_bg_texture, {401, 107}, 2, 0, 0, false);

  m_atlas->draw(m_left_door, {401, 107}, 2, m_left_door_x, 0, false);
  m_atlas->draw(m_right_door, {401, 107}, 2, m_right_door_x, 0, false);

  // ui
  GUI::draw([this]()
            { this->ui(); });

  if (m_is_playing)
    m_atlas->draw(20, 20, std::to_string(m_current_frame).c_str(), m_app->get_main_font(), {255, 255, 255});

  // gizmos stuff
  Gizmos::draw_area({m_preview_button->relative_x, m_preview_button->relative_y}, 25, m_atlas, {255, 0, 0});
  Gizmos::draw_area({m_convert_button->relative_x, m_convert_button->relative_y}, 25, m_atlas, {255, 0, 0});

  if (m_is_hovering)
  {
    SDL_SetCursor(cursor_hand);
  }
  else
  {
    SDL_SetCursor(cursor);
  }
}

std::string get_filename(std::string path)
{
  std::filesystem::path p(path);
  std::string file_name = p.filename().string();
  file_name = file_name.substr(0, file_name.find_last_of("."));
  return file_name;
}

std::string get_extension(std::string path)
{
  std::filesystem::path p(path);
  std::string extension = p.filename().string();
  extension = extension.substr(extension.find_last_of("."));
  return extension;
}

std::vector<std::string> filenames_storage;

void GameScene::input(SDL_Event event)
{
  // partial scenes
  for (auto &scene : m_partial_scenes)
  {
    scene->input(event);
  }

  switch (event.type)
  {
  case SDL_DROPFILE:
    if (m_current_extension == "")
    {
      m_current_extension = get_extension(event.drop.file);
    }

    if (m_current_extension != get_extension(event.drop.file))
    {
      m_logger->log("Please add a " + m_current_extension + " file!");
      return;
    }

    m_files_path.push_back(event.drop.file);

    filenames_storage.push_back(get_filename(event.drop.file));

    SequencerItemTypeNames[m_current_file_idx] = filenames_storage.back().c_str();
    m_timeline.myItems.push_back(Timeline::MySequenceItem{m_current_file_idx, 0 + static_cast<int>(m_gif_settings.fps * m_timeline.myItems.size()), m_gif_settings.fps + static_cast<int>(m_gif_settings.fps * m_timeline.myItems.size()), false});

    m_video_frames.push_back(GifFrame{event.drop.file, get_filename(event.drop.file), m_resources->LoadTexture(event.drop.file)});

    int width, height;

    SDL_QueryTexture(m_video_frames.back().m_texture, nullptr, nullptr, &width, &height);

    if (width >= m_gif_settings.m_width)
    {
      m_gif_settings.m_width = width;
    }

    if (height >= m_gif_settings.m_height)
    {
      m_gif_settings.m_height = height;
    }

    m_current_file_idx++;
    break;

  case SDL_MOUSEBUTTONDOWN:
    switch (event.button.button)
    {
    case SDL_BUTTON_LEFT:
      m_is_mouse_down = true;
      break;
    }

  case SDL_MOUSEBUTTONUP:
    if (event.type == SDL_MOUSEBUTTONDOWN)
      return;
    switch (event.button.button)
    {
    case SDL_BUTTON_LEFT:
      m_is_mouse_down = false;
      break;
    }

  case SDL_KEYDOWN:
    switch (event.key.keysym.scancode)
    {
    case SDL_SCANCODE_D:
      m_app->return_scene();
      break;
    case SDL_SCANCODE_F:
      // Convertion::convert(m_timeline, m_files_path, "output", "output");
      break;
    case SDL_SCANCODE_E:
      m_files_path.clear();

      /*if(m_file_path != ""){
        convert_file(m_file_path);
      }*/
      break;
    case SDL_SCANCODE_A:

      break;
    case SDL_SCANCODE_T:
      m_is_playing = !m_is_playing;
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
    switch (event.key.keysym.scancode)
    {
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
  m_gifsettings_handler->save();
}
