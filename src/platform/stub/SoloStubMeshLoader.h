//#pragma once
//
//#include "../../SoloMeshLoader.h"
//
//
//namespace solo
//{
//    class StubMeshLoader final: public MeshLoader
//    {
//    public:
//        StubMeshLoader(FileSystem* fs, ResourceManager* resourceManager);
//
//        virtual bool isLoadable(const std::string& uri) override final;
//        virtual sptr<Mesh> load(const std::string& uri) override final;
//        virtual uptr<MeshData> loadData(const std::string& uri) override final;
//    };
//}